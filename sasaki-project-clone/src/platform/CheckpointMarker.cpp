#include "CheckpointMarker.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/input.hpp>

#include <cstdlib>

using namespace godot;

void CheckpointMarker::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_area_entered", "body"), &CheckpointMarker::on_area_entered);

}

CheckpointMarker::CheckpointMarker() {
}

CheckpointMarker::~CheckpointMarker() {
}

void CheckpointMarker::_ready() {
    
    // This is an area3d
    Area3D* goal_area = get_node<Area3D>("./GoalArea");
    if (goal_area) {
        goal_area->connect("body_entered", Callable(this, "on_area_entered"));
    } else {
        UtilityFunctions::printerr("Unable to find the Goal Area in this checkpoint.");
        return;
    }
}

void CheckpointMarker::_process(double delta) {
   

}
void CheckpointMarker::on_area_entered(Node3D* body) {
    String to_compare = "Player";
    UtilityFunctions::print("Hello, I am CheckpointMarker.");
    if (body->get_name() == to_compare) {
        body->call("on_checkpoint_entered", get_global_position().z);
        StaticBody3D* child = get_node<StaticBody3D>("TowerA");
        MeshInstance3D* Amesh = child->get_node<MeshInstance3D>("MeshInstance3D");
        
        Ref<StandardMaterial3D> texture = ResourceLoader::get_singleton()->load("res://ChkptMet.tres");
        Amesh->set_material_override(texture);
    }
}
