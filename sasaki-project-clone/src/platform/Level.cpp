#include "Level.h"


#include <cstdlib>

using namespace godot;

void Level::_bind_methods() {
    ClassDB::bind_method(D_METHOD("gen_chunk", "start_offs", "noise", "amplitude", "is_biome_transition", "prev_noise", "prev_amplitude"), &Level::gen_chunk);
    ClassDB::bind_method(D_METHOD("proc_gen_mesh", "noise","prev_noise", "offset", "amplitude", "prev_amplitude", "is_biome_transition"), &Level::proc_gen_mesh);
    ClassDB::bind_method(D_METHOD("get_object_placement", "mesh_heightmap"), &Level::get_object_placement);

    ClassDB::bind_method(D_METHOD("set_chunk_size", "value"), &Level::set_chunk_size);
    ClassDB::bind_method(D_METHOD("get_chunk_size"), &Level::get_chunk_size);

    ClassDB::bind_method(D_METHOD("set_chunks_per_biome", "value"), &Level::set_chunks_per_biome);
    ClassDB::bind_method(D_METHOD("get_chunks_per_biome"), &Level::get_chunks_per_biome);

    ClassDB::bind_method(D_METHOD("set_ambient_chunk_lod", "value"), &Level::set_ambient_chunk_lod);
    ClassDB::bind_method(D_METHOD("get_ambient_chunk_lod"), &Level::get_ambient_chunk_lod);

    ClassDB::bind_method(D_METHOD("set_main_chunk_lod", "value"), &Level::set_main_chunk_lod);
    ClassDB::bind_method(D_METHOD("get_main_chunk_lod"), &Level::get_main_chunk_lod);

    ClassDB::bind_method(D_METHOD("set_obj_height_thresh", "value"), &Level::set_obj_height_thresh);
    ClassDB::bind_method(D_METHOD("get_obj_height_thresh"), &Level::get_obj_height_thresh);

    ClassDB::bind_method(D_METHOD("set_object_subdivisions", "value"), &Level::set_object_subdivisions);
    ClassDB::bind_method(D_METHOD("get_object_subdivisions"), &Level::get_object_subdivisions);

    ClassDB::bind_method(D_METHOD("on_checkpoint_cleared", "z_id"), &Level::on_checkpoint_cleared);
    ClassDB::bind_method(D_METHOD("_on_timeout"), &Level::_on_timeout); // ADDED

    ClassDB::bind_method(D_METHOD("set_compass_offset", "offset"), &Level::set_compass_offset);
    ClassDB::bind_method(D_METHOD("get_compass_offset"), &Level::get_compass_offset);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size"), "set_chunk_size", "get_chunk_size");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "chunks_per_biome"), "set_chunks_per_biome", "get_chunks_per_biome");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "ambient_chunk_lod"), "set_ambient_chunk_lod", "get_ambient_chunk_lod");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "main_chunk_lod"), "set_main_chunk_lod", "get_main_chunk_lod");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "object_height_threshold"), "set_obj_height_thresh", "get_obj_height_thresh");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "object_subdivisions"), "set_object_subdivisions", "get_object_subdivisions");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "compass_offset"), "set_compass_offset", "get_compass_offset");
    ADD_SIGNAL(MethodInfo("checkpoint_time_left", PropertyInfo(Variant::FLOAT, "time_left")));

}

Level::Level() {
}

Level::~Level() {
}

void Level::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    // Randomally initialize parameters for the first biome.
    chunk_mutex.instantiate();
    union_mutex.instantiate();
    current_noise.instantiate();
    current_noise->set_noise_type(FastNoiseLite::TYPE_PERLIN);
    current_noise->set_seed(rand() % 1048576);
    current_noise->set_fractal_octaves(3);
    current_amplitude = (rand() % 64) + 8;
}

void Level::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (first || cur_chunk_count >= chunks_per_biome-1) {
        // Store the previous biome's noise and amplitude
        Ref<FastNoiseLite> prev_noise = current_noise;

        Ref<FastNoiseLite> noise;
        current_noise.instantiate();
        current_noise->set_noise_type(FastNoiseLite::TYPE_PERLIN);
        current_noise->set_fractal_octaves(2);

        float biome_noise = 0.0;
        float biome_amplitude = 0.0;

        cur_chunk_count = 0;

        int biome = UtilityFunctions::randi() % 5;
        int prev_biome = current_biome;
        current_biome = biome;
        UtilityFunctions::print("biome is ", biome);

        // set distinct bounds for the biome parameters.
        switch (biome) {
            case BiomeSpec::biomes::RAINBOW :
                biome_noise = rand() % NOISE_RAINBOW;
                biome_amplitude = (rand() % 64) + 16;
                break;
            case BiomeSpec::biomes::LAVA :
                biome_noise = rand() % NOISE_LAVA;
                biome_amplitude = (rand() % 64) + 8;
                break;
            case BiomeSpec::biomes::PLANES :
                biome_noise = rand() % NOISE_PLANES;
                biome_amplitude = (rand() % 16) + 1;
                break;
            case BiomeSpec::biomes::OCEAN :
                biome_noise = rand() % NOISE_OCEAN;
                biome_amplitude = (rand() % 128) + 32;
                break;
            case BiomeSpec::biomes::TUNDRA :
                biome_noise = rand() % NOISE_TUNDRA;
                biome_amplitude = (rand() % 32) + 2;
                break;
        }

        float prev_amplitude = current_amplitude;
        current_noise->set_seed(biome_noise);
        current_amplitude = biome_amplitude;

        for (int i = 0; i < chunks_per_biome; i++) {
            Thread* worker = memnew(Thread);
            chunk_worker_union[cur_offs + (i * chunk_size)] = worker;

            bool is_transition = !first && (i == 1); // First chunk is transition
            worker->start(Callable(this, "gen_chunk").bind(
                cur_offs + (i * chunk_size),
                current_noise,
                current_amplitude,
                is_transition,
                prev_noise,
                prev_amplitude,
                biome, prev_biome));
        }
        first = false;
    }

    // Try to get the player. Do this in the Process because Player is dynamically instantiated 
    // by world after we enter a game.
    static bool timer_setup_done = false; // ADDED

    if (!player_found) {
        Node* player_node = get_node<Node>("/root/World/Player"); 
        if (player_node) {
            player_found = true;
            player = static_cast<RigidBody3D*>(player_node);
            player->set_global_position(Vector3(chunk_size/2, 35, chunk_size+chunk_size/ 2));
            player_spawn_location = player->get_global_position();
            player->connect("checkpoint_cleared", Callable(this, "on_checkpoint_cleared"));


            // add in the compass object.
            if (compass_obj_upgraded == nullptr) {
                Node* compass_obj;
                Ref<PackedScene> compass_packed;
                compass_packed = ResourceLoader::get_singleton()->load("res://gd-player/compass.tscn");
                compass_obj = compass_packed->instantiate();
                compass_obj_upgraded = static_cast<Node3D*>(compass_obj);
                Vector3 player_pos = player->get_global_position();
                compass_obj_upgraded->set_global_position(compass_offset);

                // Set the shader for compass object
                compass_mesh_instance = static_cast<MeshInstance3D*>(compass_obj_upgraded->get_child(0));
                if (compass_mesh_instance) {
                    shader_mat = compass_mesh_instance->get_material_override();
                    if (!shader_mat.is_valid()) {
                        shader_mat = compass_mesh_instance->get_active_material(0);
                    } else {
                        shader_mat->set_shader_parameter("angle", shader_angle);
                    }
                } else {
                    UtilityFunctions::print("Failed to find MeshInstance3D child");
                }


                add_child(compass_obj_upgraded);

                UtilityFunctions::print("Created and added compass object");
            }


            // ADDED: Initialize Timer and Label references once player is found.
            if (!timer_setup_done) {
                checkpoint_timer = get_node<Timer>("CheckpointTimer"); // Make sure you have this node
                checkpoint_timer->set_wait_time(15.0);
                checkpoint_timer->connect("timeout", Callable(this, "_on_timeout"));
                checkpoint_timer->start();
                timer_setup_done = true;
            }
        }
    } else {
        emit_signal("checkpoint_time_left", (float)checkpoint_timer->get_time_left());
        // Prevent the player from exiting the play area.
        if (player->get_global_position().x >= chunk_size) {
            player->apply_impulse(Vector3(-250, 0, 0));
        } else if (player->get_global_position().x <= 0) {
            player->apply_impulse(Vector3(250, 0, 0));
        } else if (player->get_global_position().z <= chunk_size / 4) {
            player->apply_impulse(Vector3(0, 0, 250));
        }

        if (!last_checkpoint_cleared && player->get_global_position().z - next_checkpoint_z > CHECKPOINT_WARNING_MARGIN) {
            UtilityFunctions::print("If you don't complete the last fucking checkpoint I'm going to move you back.");
            if (player->get_global_position().z - next_checkpoint_z > CHECKPOINT_ROLLBACK_MARGIN) {
                UtilityFunctions::print("Fuck you! I'm moving you to the last checkpoint.");
                if (last_checkpoint_z == 999999999) {
                    player->set_global_position(Vector3(get_global_position().x, 50, player_spawn_location.z));
                } else {

                    player->set_global_position(Vector3(last_checkpoint_x, 50, last_checkpoint_z - 30));
                }
            }
        }


        // Assistant: Handle compass pointing logic here. Rotate the Compass to point to the next checkpoint location.
        if (compass_obj_upgraded) {
            UtilityFunctions::print("======================================================================");
            
            Vector3 player_pos = player->get_global_position();
            Vector3 direction_to_checkpoint = (next_checkpoint_pos - player_pos).normalized();
            
            UtilityFunctions::print("Player Position: ", player_pos);
            UtilityFunctions::print("Next Checkpoint Position: ", next_checkpoint_pos);
            UtilityFunctions::print("Direction to Checkpoint: ", direction_to_checkpoint);

            // Move the compass object above the player (or at some offset).
            compass_obj_upgraded->set_global_position(player_pos + compass_offset);

            // If we assume we only need to rotate around the Y-axis (e.g., a compass needle on a flat plane),
            // we can compute the yaw angle needed to face the checkpoint.
            float yaw = Math::atan2(direction_to_checkpoint.x, direction_to_checkpoint.z);

            // Set the compass rotation so that it faces towards the checkpoint.
            // The vector passed to set_global_rotation is treated as Euler angles in radians.
            // We assume no pitch or roll is needed, so only yaw about the Y-axis.
            compass_obj_upgraded->set_global_rotation(Vector3(0.0f, yaw, 0.0f));
            shader_angle = yaw;
            UtilityFunctions::print("Compass Rotation (in radians): ", compass_obj_upgraded->get_global_rotation());


            // Set the shader angle
            if (compass_mesh_instance) {
                shader_mat = compass_mesh_instance->get_material_override();
                if (!shader_mat.is_valid()) {
                    shader_mat = compass_mesh_instance->get_active_material(0);
                    UtilityFunctions::print("Compass Shader Mat: ", shader_mat);
                    shader_mat->set_shader_parameter("angle", shader_angle);
                    UtilityFunctions::print("Got and Set the angle to: ", shader_angle);
                } else {
                    shader_mat->set_shader_parameter("angle", shader_angle);
                    UtilityFunctions::print("Compass set shader angle: ", shader_angle);
                }
            } else {
                compass_mesh_instance = static_cast<MeshInstance3D*>(compass_obj_upgraded->get_child(0));
                UtilityFunctions::print("Compass Mesh Instance is not valid: ", compass_mesh_instance);
            }

            // hide the checkpoint while last checkpoint_position == next_checkpoing_position
            if (last_checkpoint_cleared) {
                compass_obj_upgraded->hide();
            } else {
                compass_obj_upgraded->show();
            }


            UtilityFunctions::print("======================================================================");

        }




        // Case where we want to ADD a chunk into world.
        if (player->get_global_position().z > cur_offs) {
            // Update the current offset position for the next chunk.
            cur_offs += chunk_size;
            
            if (!chunk_worker_union.has(cur_offs)) {
                UtilityFunctions::printerr("No worker thread found for current offset!");
                return; 
            }

            Thread* desired_thread = Object::cast_to<Thread>(chunk_worker_union[cur_offs]);
            if (!desired_thread) {
                UtilityFunctions::printerr("Failed to cast to Thread for current offset!");
                return;
            }

            desired_thread->wait_to_finish();
            cur_chunk_count++;

            if (!pending_chunks.has(cur_offs)) {
                UtilityFunctions::printerr("Fatal: No pending chunk found for current offset!");
                return; 
            }

            Array chunks_at_offset = pending_chunks[cur_offs];
            for (int i = 0; i < chunks_at_offset.size(); i++) {
                Node* new_chunk = Object::cast_to<Node>(chunks_at_offset[i]);
                if (!new_chunk) {
                    UtilityFunctions::printerr("Fatal: Failed to cast pending chunk to Node!");
                    return; 
                }
                add_child(new_chunk);

                // Add transparent walls between chunks
                if (i == MAIN_CHUNK) {
                    float wall_height = 10.0f;
                    float wall_thickness = 1.0f; // Adjust as needed
                    Node3D* node_upgraded = static_cast<Node3D*>(new_chunk);
                    Vector3 chunk_position = node_upgraded->get_global_position();
                    
                    // Create left wall
                    Ref<BoxMesh> left_wall_mesh;
                    left_wall_mesh.instantiate();
                    left_wall_mesh->set_size(Vector3(wall_thickness, wall_height, chunk_size));

                    MeshInstance3D* left_wall = memnew(MeshInstance3D());
                    left_wall->set_mesh(left_wall_mesh);
                    left_wall->set_global_position(Vector3(chunk_position.x - wall_thickness / 2, 0, chunk_position.z+ (chunk_size/2)));
                    left_wall->set_material_override(create_transparent_material());
                    add_child(left_wall);

                    // Create right wall
                    Ref<BoxMesh> right_wall_mesh;
                    right_wall_mesh.instantiate();
                    right_wall_mesh->set_size(Vector3(wall_thickness, wall_height, chunk_size));

                    MeshInstance3D* right_wall = memnew(MeshInstance3D());
                    right_wall->set_mesh(right_wall_mesh);
                    right_wall->set_global_position(Vector3(chunk_position.x + chunk_size + wall_thickness / 2, 0, chunk_position.z + (chunk_size/2)));
                    right_wall->set_material_override(create_transparent_material());
                    add_child(right_wall);
                }

                // Add a checkpoint only on main chunk
                if (!first && i == MAIN_CHUNK) {
                    Vector3 checkpoint_pos = Vector3(UtilityFunctions::randi() % (chunk_size - 10) + 10, 0, cur_offs + (UtilityFunctions::randi() % (chunk_size / 2) - chunk_size / 2));
                    if (last_checkpoint_cleared) {
                        last_checkpoint_z = next_checkpoint_z;
                        last_checkpoint_x = next_checkpoint_x;
                        next_checkpoint_z = checkpoint_pos.z;
                        next_checkpoint_x = checkpoint_pos.x;
                        next_checkpoint_pos = checkpoint_pos;
                        last_checkpoint_cleared = false;
                    }
                    Ref<PackedScene> packed = ResourceLoader::get_singleton()->load("res://gd-level/checkpoint_marker.tscn");
                    if (!packed.is_valid()) {
                        UtilityFunctions::printerr("Fatal: Can't add a chunk that was registered at offset ", cur_offs);
                        return;
                    }
                    Node* obj = packed->instantiate();
                    Node3D* obj_upgraded = static_cast<Node3D*>(obj);
                    obj_upgraded->set_global_position(checkpoint_pos);
                    add_child(obj_upgraded);
                    UtilityFunctions::print("Added checkpoint_marker at i = ", i);
                }
                if (i == MAIN_CHUNK)
                {
                    first_ckpt = false;
                }
            }
        }
    }
}

Ref<StandardMaterial3D> Level::create_transparent_material() {
    Ref<StandardMaterial3D> material;
    material.instantiate();
    material->set_transparency(StandardMaterial3D::Transparency::TRANSPARENCY_MAX);
    material->set_albedo(Color(1, 1, 1, 0.2)); // Adjust alpha value for desired transparency
    return material;
}

// Make a worker thread do this.
void Level::gen_chunk(int start_offs, Ref<FastNoiseLite> noise, float amplitude, 
    bool is_biome_transition, Ref<FastNoiseLite> prev_noise, float prev_amplitude, int biome, int prev_biome) {

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    
    Array chunks;
    // We are generating two fake chunks to the left and right of the player.
    for (int chunk_idx = 0; chunk_idx < 3; chunk_idx++) {
        Ref<PackedScene> chunk_ref = ResourceLoader::get_singleton()->load("res://gd-level/ProceduralPlane.tscn");

        if (!chunk_ref.is_valid()) {
            UtilityFunctions::printerr("Unable to load the chunk level resource.");
            return;
        }
        Node* chunk = chunk_ref->instantiate();
        StaticBody3D* chunk_upgraded = static_cast<StaticBody3D*>(chunk);
        
        if (!chunk) {
            UtilityFunctions::printerr("Unable to instantiate the chunk level.");
            return;
        }
        
        chunk_upgraded->set("amplitude", amplitude);
        chunk_upgraded->set("biome", biome);
        chunk_upgraded->set("chunk_size", chunk_size);
        chunk_upgraded->set("is_biome_transition", is_biome_transition);
        chunk_upgraded->set("prev_biome", prev_biome);
        chunk_upgraded->set("prev_amplitude", prev_amplitude);
        PackedVector3Array arr;  
        PackedVector3Array obj_coords;
        if (chunk_size % ambient_chunk_lod != 0) {
            UtilityFunctions::printerr("WARNING: Your LOD must evenly divide the chunk size. You need to adjust one of those constants.");
        }

        if (chunk_idx == LEFT_VISUAL_CHUNK) {
            chunk_upgraded->set_global_position(Vector3(-chunk_size, 0, start_offs));
            arr = proc_gen_mesh(noise, prev_noise, -chunk_size, start_offs, amplitude, prev_amplitude, is_biome_transition, ambient_chunk_lod);
            chunk_upgraded->set("lod_factor", ambient_chunk_lod);

        } else if (chunk_idx == MAIN_CHUNK) {
            chunk_upgraded->set_global_position(Vector3(0, 0, start_offs));
            arr = proc_gen_mesh(noise, prev_noise, 0, start_offs, amplitude, prev_amplitude, is_biome_transition, main_chunk_lod);
            obj_coords = get_object_placement(arr);
            chunk_upgraded->set("lod_factor", main_chunk_lod);

        // right chunk
        } else {
            chunk_upgraded->set_global_position(Vector3(chunk_size, 0, start_offs));
            arr = proc_gen_mesh(noise, prev_noise, chunk_size, start_offs, amplitude, prev_amplitude, is_biome_transition, ambient_chunk_lod);
            chunk_upgraded->set("lod_factor", ambient_chunk_lod);
        }
        chunk_upgraded->set("proc_gen_vertex_arr", arr);
        chunk_upgraded->set("obj_coords", obj_coords);

        chunks.append(chunk_upgraded);
    }

    chunk_mutex->lock();
    pending_chunks[start_offs] = chunks;
    chunk_mutex->unlock();
}

PackedVector3Array Level::proc_gen_mesh(Ref<FastNoiseLite> noise, Ref<FastNoiseLite> prev_noise, 
    int x_offset, int z_offset, float amplitude, float prev_amplitude, bool is_biome_transition, int lod_factor) {
    
    PackedVector3Array vertices;
   
    for (int z = 0; z <= chunk_size; z += lod_factor) {
        float blend_factor = 0.0f;
        if (is_biome_transition) {
            blend_factor = float(z) / chunk_size; // Blend factor from 0 to 1 across the chunk
        }

        for (int x = 0; x <= chunk_size; x += lod_factor) {
            float adjusted_amplitude = amplitude * (1.0f + 0.1f * std::abs(float(x)) / chunk_size);
            float height = 0.0f;
            if (is_biome_transition) {
                float noise_value_prev = prev_noise->get_noise_2d(x + x_offset, z + z_offset);
                float noise_value_current = noise->get_noise_2d(x + x_offset, z + z_offset);
                float blended_noise = UtilityFunctions::lerp(noise_value_prev,
                    noise_value_current, blend_factor);
 
                float blended_amplitude = UtilityFunctions::lerp(prev_amplitude, 
                    adjusted_amplitude, blend_factor);
                height = blended_noise * blended_amplitude;
            } else {
                height = noise->get_noise_2d(x + x_offset, z + z_offset) * adjusted_amplitude;
            }
            vertices.push_back(Vector3(x, height, z));
        }
    }
    return vertices;
}

PackedVector3Array Level::get_object_placement(PackedVector3Array mesh_heightmap) {
    PackedVector3Array coords;
    int samples = (chunk_size / main_chunk_lod) + 1;
    for (int z = 0; z < samples - 1; z += object_subdivisions) {
        for (int x = 0; x < samples - 1; x += object_subdivisions) {
            int sample_loc = x + (z * samples); 
            Vector3 sample = mesh_heightmap[sample_loc];
            if (sample.y > object_height_threshold) {
                coords.push_back(sample);
            }   
        }
    }
    return coords;
}

void Level::on_checkpoint_cleared(int z_id) {
    UtilityFunctions::print("level was notified of the player");
    UtilityFunctions::print("We've successfully cleared the last checkpoint.");
    last_checkpoint_cleared = true;
    checkpoint_timer->start(); // Reset to 15 seconds again 
}

// ADDED: Timeout handler
void Level::_on_timeout() {
    // Reload current scene when timer runs out
    UtilityFunctions::print("Reset the timer!");
    Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_VISIBLE);
    get_tree()->reload_current_scene();
}

// Boiler plate for the exposed properties.
void Level::set_chunk_size(int p_chunk_size) {
    chunk_size = p_chunk_size;
}

int Level::get_chunk_size() const {
    return chunk_size;
}

void Level::set_chunks_per_biome(int p_chunks_per_biome) {
    chunks_per_biome = p_chunks_per_biome;
}

int Level::get_chunks_per_biome() const {
    return chunks_per_biome;
}

void Level::set_ambient_chunk_lod(int p_ambient_chunk_lod) {
    ambient_chunk_lod = p_ambient_chunk_lod;
}

int Level::get_ambient_chunk_lod() const {
    return ambient_chunk_lod;
}

void Level::set_main_chunk_lod(int p_main_chunk_lod) {
    main_chunk_lod = p_main_chunk_lod;
}

int Level::get_main_chunk_lod() const {
    return main_chunk_lod;
}

void Level::set_obj_height_thresh(int p_object_height_threshold) {
    object_height_threshold = p_object_height_threshold;
}

int Level::get_obj_height_thresh() const {
    return object_height_threshold;
}

void Level::set_object_subdivisions(int p_object_subdivisions) {
    object_subdivisions = p_object_subdivisions;
}

int Level::get_object_subdivisions() const {
    return object_subdivisions;
}

void Level::set_compass_offset(Vector3 offset) {
    compass_offset = offset;
}


Vector3 Level::get_compass_offset() {
    return compass_offset;
}