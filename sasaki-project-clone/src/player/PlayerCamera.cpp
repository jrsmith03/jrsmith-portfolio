#include "PlayerCamera.h"
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/variant/basis.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <array>


#include <godot_cpp/classes/mesh_instance3d.hpp>
using namespace godot;

void PlayerCamera::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_mouse_sensitivity", "sensitivity"), &PlayerCamera::set_mouse_sensitivity);
    ClassDB::bind_method(D_METHOD("get_mouse_sensitivity"), &PlayerCamera::get_mouse_sensitivity);
    ClassDB::bind_method(D_METHOD("set_max_pitch", "max_pitch"), &PlayerCamera::set_max_pitch);
    ClassDB::bind_method(D_METHOD("get_max_pitch"), &PlayerCamera::get_max_pitch);
    ClassDB::bind_method(D_METHOD("set_camera_distance", "camera_distance"), &PlayerCamera::set_camera_distance);
    ClassDB::bind_method(D_METHOD("get_camera_distance"), &PlayerCamera::get_camera_distance);
    ClassDB::bind_method(D_METHOD("set_camera_offset", "camera_offset"), &PlayerCamera::set_camera_offset);
    ClassDB::bind_method(D_METHOD("get_camera_offset"), &PlayerCamera::get_camera_offset);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mouse_sensitivity"), "set_mouse_sensitivity", "get_mouse_sensitivity");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_pitch"), "set_max_pitch", "get_max_pitch");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "camera_distance"), "set_camera_distance", "get_camera_distance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "camera_offset"), "set_camera_offset", "get_camera_offset");
}

PlayerCamera::PlayerCamera() {}

void PlayerCamera::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        Input* input = Input::get_singleton();
        input->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);
    }

    player = static_cast<Player*>(get_parent());
    if (player == nullptr) {
        UtilityFunctions::printerr("PlayerCamera's parent is not a Player node!");
        return;
    }

    // Get the RayCast3D node named "Clip" from the player node
    clippy = player->get_node<RayCast3D>("Clip");
    if (clippy == nullptr) {
        UtilityFunctions::printerr("Failed to find RayCast3D node 'Clip' under Player node.");
        return;
    }
    clippy->set_enabled(true);

    // Get the RayCasts from the player for edge detection
    edge_rays[0] = player->get_node<RayCast3D>("EdgeFront");
    edge_rays[1] = player->get_node<RayCast3D>("EdgeRight");
    edge_rays[2] = player->get_node<RayCast3D>("EdgeLeft");
    edge_rays[3] = player->get_node<RayCast3D>("EdgeBack");

    for(int i =0; i <4; i++){
        crouchCasts[i] = edge_rays[i]->get_global_transform().basis;
    }
}

void PlayerCamera::_process(double delta) {
    Input* input = Input::get_singleton();
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    Input* i = Input::get_singleton();
    Vector2 input_vector = i->get_vector("look_left", "look_right", "look_down", "look_up");

    // Threshold for joystick override
    const float joystick_threshold = 0.1f;

    float horizontal_input = -input_vector.x;
    float vertical_input = -input_vector.y;
    float force=input_vector.length();
    
     // Check if joystick input overrides mouse input
    if (force >= joystick_threshold) {
        // Update yaw and pitch based on joystick input
        yaw += horizontal_input * force * static_cast<float>(delta)*200 ;
        pitch -= vertical_input * force * static_cast<float>(delta)* 200;
    } else {
        // Update yaw and pitch based on mouse input
        Vector2 mouse_delta = input->get_last_mouse_velocity();
        yaw -= mouse_delta.x * mouse_sensitivity * static_cast<float>(delta);
        pitch -= mouse_delta.y * mouse_sensitivity * static_cast<float>(delta);
    }
    // Clamp the pitch to prevent flipping
    pitch = Math::clamp(pitch, -max_pitch, max_pitch);

    // Update camera rotation
    Quaternion yaw_rotation(Vector3(0, 1, 0), Math::deg_to_rad(yaw));
    Quaternion pitch_rotation(Vector3(1, 0, 0), Math::deg_to_rad(pitch));
    Quaternion final_rotation = yaw_rotation * pitch_rotation;
    Basis camera_basis = Basis(final_rotation);

    // Compute desired camera position behind the player
    Vector3 player_position = player->get_global_transform().origin;
    Vector3 forward = -camera_basis.get_column(2);
    Vector3 desired_camera_position = player_position - forward * camera_distance;

    // Set the raycast's origin to the player's position
    clippy->set_global_transform(Transform3D(Basis(), player_position));

    // Cast towards the desired camera position from the player
    clippy->set_target_position(desired_camera_position - player_position);

    // Update the raycast to get the latest collision info
    clippy->force_raycast_update();

    // Check for collision
    if (clippy->is_colliding()) {
        Vector3 collision_point = clippy->get_collision_point();
        // Offset to prevent the camera from clipping into objects
        float collision_offset = 0.1f;
        Vector3 direction = (desired_camera_position - player_position).normalized();
        Vector3 new_camera_position = collision_point - direction * collision_offset;
        set_global_transform(Transform3D(camera_basis, new_camera_position));
    } else {
        // No collision, position the camera at the desired position
        set_global_transform(Transform3D(camera_basis, desired_camera_position));
    }

    // Edge detection

    if (input->is_action_pressed("crawl")) {
        int num_collisions = 0;
        std::array<bool, 4> collisions_hit;
        for (int edge = 0; edge < 4; edge++) {
            if (edge_rays[edge]->is_colliding()) {
                num_collisions++;
                collisions_hit[edge] = true;
            } else {
                collisions_hit[edge] = false;
    

            }
        }
        // THis is a less poor heuristic but still bad
        if (num_collisions == 3 || num_collisions == 2) {
            for (int edge = 0; edge < 4; edge++) {
                if (!collisions_hit[edge]) {
                    if (edge == 0 || edge == 3) {
                        player->set_linear_velocity(Vector3(0, player->get_linear_velocity()[1], player->get_linear_velocity()[2]));
                    } else {
                        player->set_linear_velocity(Vector3(player->get_linear_velocity()[0], player->get_linear_velocity()[1], 0));

                    }
                }
            }
        }
    }
    
    // RayCast3D* downCast = nullptr;

    // if (downCast) {
    //     // Reset the RayCast3D's global rotation each frame
    //     Transform3D raycast_transform = downCast->get_global_transform();
    //     raycast_transform.basis = initial_downCast_basis;
    //     downCast->set_global_transform(raycast_transform);
    // }
    for(int i =0; i <4; i++){
        Transform3D edgecast_transform = edge_rays[i]->get_global_transform();
        edgecast_transform.basis = crouchCasts[i];
        edge_rays[i]->set_global_transform(edgecast_transform);
    }
}


void PlayerCamera::update_camera_rotation() {
    Quaternion yaw_rotation(Vector3(0, 1, 0), Math::deg_to_rad(yaw));
    Quaternion pitch_rotation(Vector3(1, 0, 0), Math::deg_to_rad(pitch));
    Quaternion final_rotation = yaw_rotation * pitch_rotation;

    Transform3D transform = get_global_transform();
    transform.basis = Basis(final_rotation);
    set_global_transform(transform);
}

void PlayerCamera::set_mouse_sensitivity(float sensitivity) {
    mouse_sensitivity = sensitivity;
}

float PlayerCamera::get_mouse_sensitivity() const {
    return mouse_sensitivity;
}

void PlayerCamera::set_max_pitch(float max_pitch_value) {
    max_pitch = max_pitch_value;
}

float PlayerCamera::get_max_pitch() const {
    return max_pitch;
}

void PlayerCamera::set_camera_offset(float new_camera_offset) {
    camera_offset = new_camera_offset;
}
float PlayerCamera::get_camera_offset() const{
    return camera_offset;
}

void PlayerCamera::update_camera_position() {
    Transform3D camera_transform = this->get_transform();
    // Extract the forward direction from the basis (Z axis)
    Vector3 forward = -camera_transform.basis.get_column(2); // Forward is negative Z axis
    
    this->set_position(Vector3(-camera_distance*forward.x+camera_offset,-camera_distance*forward.y+camera_offset,-camera_distance*forward.z));
}

void PlayerCamera::set_camera_distance(float new_camera_distance){
    camera_distance = new_camera_distance;
}
float PlayerCamera::get_camera_distance() const{
    return camera_distance;
}

