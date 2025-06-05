#ifndef LEVEL_H
#define LEVEL_H

#include "godot_cpp/classes/node3d.hpp"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/fast_noise_lite.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/mutex.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/concave_polygon_shape3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/label.hpp> // ADDED
#include <godot_cpp/classes/timer.hpp> // ADDED
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/thread.hpp>
// #include <godot_cpp/classes/utility_functions.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "BiomeSpec.h"

#define CHUNKS_PER_GEN 3
#define LEFT_VISUAL_CHUNK 0
#define RIGHT_VISUAL_CHUNK 2
#define MAIN_CHUNK 1

// Noise Constants
#define NOISE_LAVA  1256613
#define NOISE_RAINBOW 562261
#define NOISE_PLANES 551963
#define NOISE_OCEAN 651123
#define NOISE_TUNDRA 5647


#define CHECKPOINT_WARNING_MARGIN 50     // Warn the player to do the last CKPT.
#define CHECKPOINT_ROLLBACK_MARGIN 150     // Move the player back to the last chunk.

namespace godot {

class Level : public Node3D {
	GDCLASS(Level, Node3D)

public:
	Level();
	~Level();
	void _on_timeout();
	int cur_offs = 0;

	bool player_found = false;
	RigidBody3D* player = nullptr;
	Dictionary pending_chunks;
	Dictionary chunk_worker_union;

	void _ready() override;
	void _process(double delta) override;
	void gen_chunk(int start_offs, Ref<FastNoiseLite> noise, float amplitude, 
		bool is_biome_transition, Ref<FastNoiseLite> prev_noise, float prev_amplitude, int biome, int prev_biome);

	Ref<StandardMaterial3D> create_transparent_material();

	PackedVector3Array proc_gen_mesh(Ref<FastNoiseLite> noise, Ref<FastNoiseLite> prev_noise, 
		int x_offset, int z_offset, float amplitude, float prev_amplitude, bool is_biome_transition, int lod_factor);
		
	PackedVector3Array get_object_placement(PackedVector3Array mesh_heightmap);
	void on_checkpoint_cleared(int z_id);

	// "Prevent" races and deadlocks
	Ref<Mutex> chunk_mutex;
	Ref<Mutex> union_mutex;

	// Variables
	int cur_chunk_count = 0;
	bool first = true;
	bool first_ckpt = true;
	int chunk_size = 128;               // Size of the chunk
    int chunks_per_biome = 3;         // Chunks per biome
    int ambient_chunk_lod = 8;        // Ambient LOD for chunk
    int main_chunk_lod = 4;           // Main LOD for chunk
    int object_height_threshold = 3;  // Threshold for object generation
    int object_subdivisions = 20;      // Object subdivisions for detail

	// Keep track of the goals
	int next_checkpoint_z = 999999999;
	int next_checkpoint_x = 999999999;
	int last_checkpoint_z = 0;
	int last_checkpoint_x = 0;
	Vector3 next_checkpoint_pos;

	bool last_checkpoint_cleared = true;

	//Vector3 last_checkpoint_position = Vector3();

	Vector3 player_spawn_location;

	Node3D* compass_obj_upgraded = nullptr;
	Vector3 compass_offset = Vector3(0, 0, 0);
	MeshInstance3D* compass_mesh_instance = nullptr;
	Ref<ShaderMaterial> shader_mat;
	float shader_angle = 0;

	// properties
	
	int current_biome = -1;
	void set_chunk_size(int p_chunk_size);
    int get_chunk_size() const;

    void set_chunks_per_biome(int p_chunks_per_biome);
    int get_chunks_per_biome() const;

    void set_ambient_chunk_lod(int p_ambient_chunk_lod);
    int get_ambient_chunk_lod() const;

    void set_main_chunk_lod(int p_main_chunk_lod);
    int get_main_chunk_lod() const;

    void set_obj_height_thresh(int p_object_height_threshold);
    int get_obj_height_thresh() const;

    void set_object_subdivisions(int p_object_subdivisions);
    int get_object_subdivisions() const;

	void set_compass_offset(Vector3 offset);
	Vector3 get_compass_offset();

protected:
	static void _bind_methods();

private:
	Ref<FastNoiseLite> current_noise;
	float current_amplitude;

	Timer* checkpoint_timer = nullptr;  // ADDED: Pointer to the dynamically created timer



};
}
#endif
