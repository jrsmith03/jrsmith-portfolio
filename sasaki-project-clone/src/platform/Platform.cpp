#include "Platform.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/input.hpp>

#include <cstdlib>

using namespace godot;

void Platform::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_wallhang_enter", "area"), &Platform::on_wallhang_enter);
    ClassDB::bind_method(D_METHOD("on_wallhang_exit", "area"), &Platform::on_wallhang_exit);
}

Platform::Platform() {
}

Platform::~Platform() {
}

void Platform::_ready() {
    must_update_hang_state = true;
    area_occupied = false;
    player = nullptr;

    wall_hang_area = get_node<Area3D>("./WallHangArea");
    if (wall_hang_area) {
        wall_hang_area->connect("area_entered", Callable(this, "on_wallhang_enter"));
        wall_hang_area->connect("area_exited", Callable(this, "on_wallhang_exit"));
    }
}

void Platform::on_wallhang_enter(Area3D *area) {
    player = area->get_parent();
    // Only set the linear velocity to zero once to allow player movement
    RigidBody3D *player_body = Object::cast_to<RigidBody3D>(player);
    if (player_body && Input::get_singleton()->is_action_pressed("hang")) {
        player_body->set_linear_velocity(Vector3(0, 0, 0));
    }
    
    area_occupied = true;
}

void Platform::on_wallhang_exit(Area3D *area) {
    area_occupied = false;

    player = area->get_parent();
}

void Platform::_process(double delta) {
    // JRS: I wanted to make it so that 'h' needs to be pressed for the duration of the
    // wall hang. To do this, we need to check the input condition each frame and
    // move to an exit state upon release (or upon area exit). I achieve this
    // by using the signals to update area_entered bool and checking that state every frame
    if (area_occupied && player && Input::get_singleton()->is_action_pressed("hang")) {
        RigidBody3D *player_body = Object::cast_to<RigidBody3D>(player);
        if (player_body) {
            // Freeze player by setting velocities to zero and disabling gravity
            player_body->set_gravity_scale(0);
        }
        must_update_hang_state = true;
    } else if (must_update_hang_state && player && player->is_class("RigidBody3D")) {
        must_update_hang_state = false;
        RigidBody3D *player_body = Object::cast_to<RigidBody3D>(player);
        if (player_body) {
            // Restore player’s gravity and reset damping
            player_body->set_gravity_scale(10); // Set to your normal gravity scale
        }
    }
}
