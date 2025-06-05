#include "ProceduralPlane.h"
#include <numeric>
#include <algorithm>


using namespace godot;

void ProceduralPlane::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_array"), &ProceduralPlane::set_array);
    ClassDB::bind_method(D_METHOD("get_array"), &ProceduralPlane::get_array);
    ClassDB::bind_method(D_METHOD("set_object_coords"), &ProceduralPlane::set_object_coords);
    ClassDB::bind_method(D_METHOD("get_object_coords"), &ProceduralPlane::get_object_coords);\
    ClassDB::bind_method(D_METHOD("set_amplitude"), &ProceduralPlane::set_amplitude);
    ClassDB::bind_method(D_METHOD("get_amplitude"), &ProceduralPlane::get_amplitude);
    ClassDB::bind_method(D_METHOD("set_biome"), &ProceduralPlane::set_biome);
    ClassDB::bind_method(D_METHOD("get_biome"), &ProceduralPlane::get_biome);
    ClassDB::bind_method(D_METHOD("set_lod"), &ProceduralPlane::set_lod);
    ClassDB::bind_method(D_METHOD("get_lod"), &ProceduralPlane::get_lod);

    ClassDB::bind_method(D_METHOD("set_chunk_size", "value"), &ProceduralPlane::set_chunk_size);
    ClassDB::bind_method(D_METHOD("get_chunk_size"), &ProceduralPlane::get_chunk_size);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size"), "set_chunk_size", "get_chunk_size");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "proc_gen_vertex_arr"), "set_array", "get_array");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "obj_coords"), "set_object_coords", "get_object_coords");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "amplitude"), "set_amplitude", "get_amplitude");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "biome"), "set_biome", "get_biome");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_factor"), "set_lod", "get_lod");

    ClassDB::bind_method(D_METHOD("set_prev_biome", "value"), &ProceduralPlane::set_prev_biome);
    ClassDB::bind_method(D_METHOD("get_prev_biome"), &ProceduralPlane::get_prev_biome);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "prev_biome"), "set_prev_biome", "get_prev_biome");

    ClassDB::bind_method(D_METHOD("set_is_biome_transition", "value"), &ProceduralPlane::set_is_biome_transition);
    ClassDB::bind_method(D_METHOD("get_is_biome_transition"), &ProceduralPlane::get_is_biome_transition);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_biome_transition"), "set_is_biome_transition", "get_is_biome_transition");

    ClassDB::bind_method(D_METHOD("set_prev_amplitude", "value"), &ProceduralPlane::set_prev_amplitude);
    ClassDB::bind_method(D_METHOD("get_prev_amplitude"), &ProceduralPlane::get_prev_amplitude);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "prev_amplitude"), "set_prev_amplitude", "get_prev_amplitude");

    ClassDB::bind_method(D_METHOD("set_r_constants", "constants"), &ProceduralPlane::set_r_constants);
    ClassDB::bind_method(D_METHOD("get_r_constants"), &ProceduralPlane::get_r_constants);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "r_constants"), "set_r_constants", "get_r_constants");

    ClassDB::bind_method(D_METHOD("set_g_constants", "constants"), &ProceduralPlane::set_g_constants);
    ClassDB::bind_method(D_METHOD("get_g_constants"), &ProceduralPlane::get_g_constants);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "g_constants"), "set_g_constants", "get_g_constants");

    ClassDB::bind_method(D_METHOD("set_b_constants", "constants"), &ProceduralPlane::set_b_constants);
    ClassDB::bind_method(D_METHOD("get_b_constants"), &ProceduralPlane::get_b_constants);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "b_constants"), "set_b_constants", "get_b_constants");

}

ProceduralPlane::ProceduralPlane() {
}

ProceduralPlane::~ProceduralPlane() {
}


void ProceduralPlane::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    //is_biome_transition = true;
    Ref<PhysicsMaterial> physics_material;
    physics_material.instantiate();

    switch (biome) {
        case BiomeSpec::biomes::RAINBOW :
            //UtilityFunctions::print("Rainbow biome!");
            r_multiplier = 1;
            g_multiplier = 1;
            b_multiplier = 1;
            physics_material->set_friction(1.0);  // Set desired friction
            physics_material->set_bounce(0.1);    // Set desired bounciness

            break;
        case BiomeSpec::biomes::LAVA :
            //UtilityFunctions::print("LAVA biome!");
            r_multiplier = 1;
            g_multiplier = .647;
            b_multiplier = 0.01;
            physics_material->set_friction(20);  // Set desired friction
            physics_material->set_bounce(0.1);    // Set desired bounciness

            break;
        case BiomeSpec::biomes::TUNDRA :
            //UtilityFunctions::print("TUNDRA biome!");
            r_multiplier = 0.05;
            g_multiplier = 0.045;
            b_multiplier = 1;
            physics_material->set_friction(1.0);  // Set desired friction
            physics_material->set_bounce(3);    // Set desired bounciness


            break;
        case BiomeSpec::biomes::OCEAN :
            //UtilityFunctions::print("OCEAN biome!");
            r_multiplier = .5;
            g_multiplier = .5;
            b_multiplier = .5;
            physics_material->set_friction(.001);  // Set desired friction
            physics_material->set_bounce(0.25);    // Set desired bounciness


            break;
    }

    UtilityFunctions::print("Biome is ", biome);
    UtilityFunctions::print("Prev biome is ", prev_biome);
    if (prev_biome != -1) {
        get_biome_colors(prev_biome, prev_r_multiplier, prev_g_multiplier, prev_b_multiplier);
    }
    
    

    set_physics_material_override(physics_material);
    MeshInstance3D *mesh_instance = Object::cast_to<MeshInstance3D>(get_node<MeshInstance3D>("ProceduralPlaneMesh"));

    Ref<SurfaceTool> tool;
    tool.instantiate();
    tool->begin(Mesh::PRIMITIVE_TRIANGLES);

    // Keep track of the INDEX into the pgva. Each INDEX will correspond to
    // a different WORLD COORDINATE. 
    int samples = (chunk_size / lod) + 1;
    for (int lod_z = 0; lod_z < samples - 1; lod_z++) {
        for (int lod_x = 0; lod_x < samples - 1; lod_x++) {
            // Calculate indices for the vertices
            int top_left = lod_x + (lod_z * samples);        // Current vertex
            int top_right = (lod_x + 1) + (lod_z * samples); // Right neighbor
            int bottom_left = lod_x + ((lod_z + 1) * samples); // Below neighbor
            int bottom_right = (lod_x + 1) + ((lod_z + 1) * samples); // Diagonal neighbor
             int max_z_val = chunk_size;
            // First triangle: top-left, top-right, bottom-left
            set_heightmap_color(tool, pgva[top_left], max_z_val);
            tool->add_vertex(pgva[top_left]);

            set_heightmap_color(tool, pgva[top_right], max_z_val);
            tool->add_vertex(pgva[top_right]);


            set_heightmap_color(tool, pgva[bottom_left], max_z_val);
            tool->add_vertex(pgva[bottom_left]);
            
            // Second triangle: top-right, bottom-right, bottom-left
            set_heightmap_color(tool, pgva[top_right], max_z_val);
            tool->add_vertex(pgva[top_right]);
            
            set_heightmap_color(tool, pgva[bottom_right], max_z_val);
            tool->add_vertex(pgva[bottom_right]);

            set_heightmap_color(tool, pgva[bottom_left], max_z_val);
            tool->add_vertex(pgva[bottom_left]);
        }
    }


    
    tool->generate_normals();

    Ref<ArrayMesh> array_mesh;
    array_mesh.instantiate();
    tool->commit(array_mesh);

    Ref<StandardMaterial3D> material;
    material.instantiate();
    material->set_flag(StandardMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
    material->set_metallic(.5);
    material->set_emission(Color(0, 1, 0));
    material->set_emission_intensity(5);

    mesh_instance->set_mesh(array_mesh);
    mesh_instance->set_material_override(material);

    CollisionShape3D *collider = get_node<CollisionShape3D>("Collider");
    Ref<ConcavePolygonShape3D> polygon_shape = array_mesh->create_trimesh_shape();
    // Ref<StaticBody3D> physics_body = array_mesh->create_trimesh_shape();
    collider->set_shape(polygon_shape);
    

    add_objects();
}

void ProceduralPlane::get_biome_colors(int biome, float &r, float &g, float &b) {
    switch (biome) {
        case BiomeSpec::biomes::RAINBOW:
            r = 1.0f; g = 1.0f; b = 1.0f;
            break;
        case BiomeSpec::biomes::LAVA:
            r = 1.0f; g = 0.647f; b = 0.0f;
            break;
        case BiomeSpec::biomes::TUNDRA:
            r = 0.05f; g = 0.05f; b = 1.0f;
            break;
        case BiomeSpec::biomes::OCEAN:
            r = 0.5f; g = 0.5f; b = 0.5f;
            break;
        default:
            // Default fallback
            r = 1.0f; g = 1.0f; b = 1.0f;
            break;
    }
}



void ProceduralPlane::_process(double delta) {
}

void ProceduralPlane::set_array (PackedVector3Array value) { 
    pgva = value.duplicate(); 
}
PackedVector3Array ProceduralPlane::get_array() { return pgva; }

void ProceduralPlane::set_object_coords (PackedVector3Array value) { 
    object_coords = value.duplicate(); 
}
PackedVector3Array ProceduralPlane::get_object_coords() { return object_coords; }

void ProceduralPlane::set_amplitude (float value) { 
    amplitude = value; 
}
float ProceduralPlane::get_amplitude() { return amplitude; }

void ProceduralPlane::set_biome (int value) { 
    biome = value; 
}
int ProceduralPlane::get_biome() { return biome; }

void ProceduralPlane::set_lod (int value) { 
    lod = value; 
}
int ProceduralPlane::get_lod() { return lod; }

void ProceduralPlane::set_chunk_size(int p_chunk_size) {
    chunk_size = p_chunk_size;
}

int ProceduralPlane::get_chunk_size() const {
    return chunk_size;
}

void ProceduralPlane::set_prev_biome(int p_prev_biome) {
    prev_biome = p_prev_biome;
}

int ProceduralPlane::get_prev_biome() const {
    return prev_biome;
}

void ProceduralPlane::set_is_biome_transition(bool p_is_biome_transition) {
    is_biome_transition = p_is_biome_transition;
}

bool ProceduralPlane::get_is_biome_transition() const {
    return is_biome_transition;
}

void ProceduralPlane::set_prev_amplitude(float value) {
    prev_amplitude = value;
}

float ProceduralPlane::get_prev_amplitude() const {
    return prev_amplitude;
}



// Will instantiate and add an object corresponding to the specified path.
void ProceduralPlane::add_objects() {
    if (object_coords.size() > MAX_OBJECT_COUNT_PER_CHUNK) {
        UtilityFunctions::printerr("Error: too many objects requested. \
            Either modify the density or increase object count.");
        return;
    }
    for (Vector3 coord : object_coords) {

        float rand_gen = UtilityFunctions::randf();
        // int rand_gen = 0;

        Node* obj;
        Ref<PackedScene> packed;

        if (rand_gen < 0.45) {
            // Coin 45%
            packed = ResourceLoader::get_singleton()->load("res://gd-gameobj/Coin.tscn");
        } else if (rand_gen > 0.45 && rand_gen < 0.80) {
            // Powerup 20%
            packed = ResourceLoader::get_singleton()->load("res://gd-gameobj/Coin.tscn");
        } else if (rand_gen > 0.80 && rand_gen < 0.90) {
            // Shooter 20%
            packed = ResourceLoader::get_singleton()->load("res://gd-ai/slime_agent.tscn");
        } else {
            // Kirby 15%
            packed = ResourceLoader::get_singleton()->load("res://gd-ai/shoot_agent.tscn");

        }

        obj = packed->instantiate();
        Node3D* obj_upgraded = static_cast<Node3D*>(obj);
        obj_upgraded->set_global_position(coord);
        obj_count++;
        add_child(obj_upgraded);
    }
}

// lava: g is .647, b is 0
void ProceduralPlane::set_heightmap_color(Ref<SurfaceTool> tool, Vector3 vertex, int max_val) {
    float blend_factor = 0.0f;
    if (is_biome_transition && prev_biome != -1) {
        blend_factor = vertex.z / float(chunk_size);
    }

    float normalized_y = (vertex.y + amplitude) / amplitude;
    float frequency = 6.28318530718f; // 2*pi

    // Current biome colors
    Color cur_color = calculate_biome_color(frequency, normalized_y, r_multiplier, g_multiplier, b_multiplier);
    float cur_red   = cur_color.r;
    float cur_green = cur_color.g;
    float cur_blue  = cur_color.b;

    float red = cur_red;
    float green = cur_green;
    float blue = cur_blue;

    // Only compute previous biome colors if we have a valid previous biome and are blending
    if (is_biome_transition && prev_biome != -1) {
        float normalized_y_p = (vertex.y + prev_amplitude) / prev_amplitude;
        Color prev_color = calculate_biome_color(frequency, normalized_y_p, prev_r_multiplier, prev_g_multiplier, prev_b_multiplier);
        float prev_red   = prev_color.r;
        float prev_green = prev_color.g;
        float prev_blue  = prev_color.b;

        // Blend the colors
        red   = UtilityFunctions::lerp(prev_red, cur_red, blend_factor);
        green = UtilityFunctions::lerp(prev_green, cur_green, blend_factor);
        blue  = UtilityFunctions::lerp(prev_blue, cur_blue, blend_factor);
    }

    tool->set_color(Color(red, green, blue));
}

Color ProceduralPlane::calculate_biome_color(float frequency, float normalized_y, float r_mult, float g_mult, float b_mult) {
    float red   = 0.5f * (sin(r_mult * frequency * normalized_y + r_constants.x) + r_constants.y);
    float green = 0.5f * (sin(g_mult * frequency * normalized_y + g_constants.x) + g_constants.y);
    float blue  = 0.5f * (sin(b_mult * frequency * normalized_y + b_constants.x) + b_constants.y);

    return Color(red, green, blue);
}


void ProceduralPlane::set_r_constants(Vector2 constants) {
    r_constants = constants;
}

Vector2 ProceduralPlane::get_r_constants() {
    return r_constants;
}

void ProceduralPlane::set_g_constants(Vector2 constants) {
    g_constants = constants;
}

Vector2 ProceduralPlane::get_g_constants() {
    return g_constants;
}

void ProceduralPlane::set_b_constants(Vector2 constants) {
    b_constants = constants;
}

Vector2 ProceduralPlane::get_b_constants() {
    return b_constants;
}