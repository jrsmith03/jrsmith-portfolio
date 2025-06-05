#ifndef PLAYER_CAMERA_H
#define PLAYER_CAMERA_H

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include "Player.h"

#include <array>

using namespace godot;

class PlayerCamera : public Camera3D {
    GDCLASS(PlayerCamera, Camera3D);

protected:
    static void _bind_methods();

private:
    float mouse_sensitivity = 0.2f;
    float max_pitch = 89.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float camera_distance = 0.0f;
    float camera_offset = 0.0f;
    Basis initial_downCast_basis;
    Basis crouchCasts[4];
    

public:
    PlayerCamera();

    void _ready() override;
    void _process(double delta) override;
    void update_camera_rotation();

    void set_mouse_sensitivity(float sensitivity);
    float get_mouse_sensitivity() const;

    void set_max_pitch(float max_pitch_value);
    float get_max_pitch() const;

    void set_camera_distance(float new_camera_distance);
    float get_camera_distance() const;

    void set_camera_offset(float new_camera_offset);
    float get_camera_offset() const;

    void update_camera_position();

	Player* player;
    RayCast3D* clippy;

    // Edge detection rays
    std::array<RayCast3D*, 4> edge_rays;
};

#endif // PLAYER_CAMERA_H

