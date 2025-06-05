#ifndef ProceduralPlane_H
#define ProceduralPlane_H

#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include "godot_cpp/classes/shape3d.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/plane_mesh.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/classes/concave_polygon_shape3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/classes/physics_material.hpp>

#include <godot_cpp/classes/resource_loader.hpp>
#include "BiomeSpec.h"

#include "Level.h"
#define MAX_OBJECT_COUNT_PER_CHUNK 102800

namespace godot {

class ProceduralPlane : public StaticBody3D {
	GDCLASS(ProceduralPlane, StaticBody3D)

public:
	ProceduralPlane();
	~ProceduralPlane();
	void _ready() override;
	void _process(double delta) override;
	// easier to pass int type. we compare to the enum. see BiomeSpec.h
	int biome;
	int lod = 1; // default detail

	int obj_count = 0;
	PackedVector3Array pgva;
	PackedVector3Array object_coords;
	float amplitude = 10;
	float prev_amplitude = 10;
	void set_prev_amplitude(float value);
    float get_prev_amplitude() const;
	void set_heightmap_color(Ref<SurfaceTool> tool, Vector3 vertex, int max_val);
	void add_objects();
	void set_array (PackedVector3Array value);
	PackedVector3Array get_array();
	void set_object_coords (PackedVector3Array value);
	PackedVector3Array get_object_coords();
	void set_amplitude (float value);
	float get_amplitude();
	void set_biome (int value);
	int get_biome();
	void set_lod (int value);
	int get_lod();
	void set_chunk_size(int p_chunk_size);
    int get_chunk_size() const;
	int chunk_size = 128;
	std::vector<float> blend_factors;

	void get_biome_colors(int biome, float &r, float &g, float &b);
	std::vector<MeshInstance3D*> rock_objects;
	float r_multiplier = 1;
	float g_multiplier = 1;
	float b_multiplier = 1;
	float prev_r_multiplier;
	float prev_g_multiplier;
	float prev_b_multiplier;

	Vector2 r_constants = Vector2(4.0f, 1);
	Vector2 g_constants = Vector2(2.094f, 1);
	Vector2 b_constants = Vector2(8.0f, 1);



	bool rainbow = false;

	int prev_biome;
	bool is_biome_transition;

	void set_prev_biome(int p_prev_biome);
	int get_prev_biome() const;

	void set_is_biome_transition(bool p_is_biome_transition);
	bool get_is_biome_transition() const;

	void set_r_constants(Vector2 constants);
	Vector2 get_r_constants();

	void set_g_constants(Vector2 constants);
	Vector2 get_g_constants();

	void set_b_constants(Vector2 constants);
	Vector2 get_b_constants();

	Color calculate_biome_color(float frequency, float normalized_y, float r_mult, float g_mult, float b_mult);




protected:
	static void _bind_methods();

private:

};

}
#endif
