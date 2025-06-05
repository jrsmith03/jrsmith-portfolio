#ifndef CheckpointMarker_H
#define CheckpointMarker_H

#include "godot_cpp/classes/node.hpp"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

class CheckpointMarker : public Node3D {
	GDCLASS(CheckpointMarker, Node3D)

public:
	CheckpointMarker();
	~CheckpointMarker();
	void on_area_entered(Node3D* body);

	void _ready() override;
	void _process(double delta) override;

protected:
	static void _bind_methods();

private:

};

}
#endif
