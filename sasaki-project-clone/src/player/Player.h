#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include "godot_cpp/classes/rigid_body3d.hpp"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "game_obj/Coin.h"

#include <cstdlib>
#include <cmath>

namespace godot {

class Player : public RigidBody3D {
	GDCLASS(Player, RigidBody3D)

public:
	Player();
	~Player();

	void _ready() override;
	void _process(double delta) override;
	void _input(const Ref<InputEvent>& event) override;
	void _physics_process();
	void on_ground_collision(Node* body);
	void glide_timeout();
	Vector3 get_forward_direction(Camera3D* camera);
	Vector3 get_right_direction(Camera3D* camera);
  
	float get_normal_gravity_scale() const;
	void set_normal_gravity_scale(const float new_normal_gravity_scale);
  
	float get_glide_gravity_scale() const;
	void set_glide_gravity_scale(const float new_glide_gravity_scale);
  
	int get_jumps_allowed() const;
	void set_jumps_allowed(const int new_jumps_allowed);

	float get_player_speed() const;
	void set_player_speed(const float new_player_speed);
  
	void set_walkable_angle(const int new_walkable_angle);
	float get_walkable_angle() const;

	void set_air_control(float new_air_control);
	float get_air_control();

	void set_air_resistance(float new_air_resistance);
	float get_air_resistance();

	void set_camera_mode(bool new_camera_mode);
	bool get_camera_mode();

	void on_checkpoint_entered(int pos);

	Vector3 get_player_forward_direction();
	Vector3 get_player_right_direction();

	int score = 0;

	void on_update_score(int value);
	void on_glider_equip();

	void stop_group_sound(const String& group_name);
	void pause_group_sound(const String& group_name);
	void resume_group_sound(const String& group_name);
    void play_collision_audio(const String& sound_path, const String& group_name);
	void on_audio_finished(AudioStreamPlayer *sound);
	void on_audio_paused(AudioStreamPlayer *sound);
	
	double get_current_time_in_seconds();
	bool has_glider = false;
	bool is_grounded = true;

protected:
	static void _bind_methods();

private:
	Vector3 direction = Vector3();
    Basis initial_downCast_basis;
	RigidBody3D* player_body;
	Camera3D* player_camera;
	Engine* state;
	Timer* glide_timer;
	double player_speed = 0.5;
	double DEFAULT_GLIDE_TIME = 2.0f;
	Vector3 velocity;
	int jumps_allowed = 1;
	int MAX_JUMPS = 1;
	float air_control = .99;
	float air_resistance;
	bool is_dead = false;
	bool is_gliding = false;
	bool can_glide = false; 
	bool glide_timer_started = false;
	// bool camera_follow_mode = true;
	bool mute_sfx = false;
	float normal_gravity_scale = 10.0f;
	float glide_gravity_scale = 0.5f;
	float walkable_angle = 45;
	double last_bounce = 0;

	Vector3 prev_position;
	MeshInstance3D* gliderPlaceholder;
	Vector3 glider_gpos;
};


}
#endif
