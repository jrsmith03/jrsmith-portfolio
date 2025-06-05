#include "Player.h"

using namespace godot;

void Player::_bind_methods() {
    ClassDB::bind_method(D_METHOD("glide_timeout"),
       &Player::glide_timeout);

    ClassDB::bind_method(D_METHOD("on_glider_equip"),
       &Player::on_glider_equip);

    // Bind speed fields
    ClassDB::bind_method(D_METHOD("get_air_control"),
         &Player::get_air_control);
    ClassDB::bind_method(D_METHOD("set_air_control", "new_air_control"), 
        &Player::set_air_control);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_control"), 
        "set_air_control", "get_air_control");

    ClassDB::bind_method(D_METHOD("get_air_resistance"),
         &Player::get_air_resistance);
    ClassDB::bind_method(D_METHOD("set_air_resistance", "new_air_resistance"), 
        &Player::set_air_resistance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_resistance"), 
        "set_air_resistance", "get_air_resistance");


    ClassDB::bind_method(D_METHOD("get_player_speed"),
         &Player::get_player_speed);
    ClassDB::bind_method(D_METHOD("set_player_speed", "new_player_speed"), 
        &Player::set_player_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_speed"), 
        "set_player_speed", "get_player_speed");

    // Bind gravity fields
    ClassDB::bind_method(D_METHOD("get_normal_gravity_scale"),
        &Player::get_normal_gravity_scale);
    ClassDB::bind_method(D_METHOD("set_normal_gravity_scale", "new_normal_gravity_scale"),
        &Player::set_normal_gravity_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "normal_gravity_scale"), 
        "set_normal_gravity_scale", "get_normal_gravity_scale");

    ClassDB::bind_method(D_METHOD("get_glide_gravity_scale"),
        &Player::get_glide_gravity_scale);
    ClassDB::bind_method(D_METHOD("set_glide_gravity_scale", "new_glide_gravity_scale"),
        &Player::set_glide_gravity_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "glide_gravity_scale"), 
        "set_glide_gravity_scale", "get_glide_gravity_scale");

    // Bind jumps_allowed
    ClassDB::bind_method(D_METHOD("get_jumps_allowed"),
        &Player::get_jumps_allowed);
    ClassDB::bind_method(D_METHOD("set_jumps_allowed", "new_jumps_allowed"),
        &Player::set_jumps_allowed);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "jumps_allowed"), 
        "set_jumps_allowed", "get_jumps_allowed");

    // Bind walkable angle
    ClassDB::bind_method(D_METHOD("get_walkable_angle"),
        &Player::get_walkable_angle);
    ClassDB::bind_method(D_METHOD("set_walkable_angle", "new_walkable_angle"),
        &Player::set_walkable_angle);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "walkable_angle"), 
        "set_walkable_angle", "get_walkable_angle");


    // bind on update score
    ClassDB::bind_method(D_METHOD("on_update_score", "value"),
        &Player::on_update_score);
    ClassDB::bind_method(D_METHOD("on_checkpoint_entered", "value"),
        &Player::on_checkpoint_entered);
        


    ADD_SIGNAL(MethodInfo("score_updated", PropertyInfo(Variant::INT, "new_score")));
    ADD_SIGNAL(MethodInfo("glide_time_left", PropertyInfo(Variant::FLOAT, "time_left")));
    ADD_SIGNAL(MethodInfo("death"));
    ADD_SIGNAL(MethodInfo("speed_updated", PropertyInfo(Variant::FLOAT, "new_speed")));
    ADD_SIGNAL(MethodInfo("is_grounded"));
    ADD_SIGNAL(MethodInfo("checkpoint_cleared", PropertyInfo(Variant::INT, "z_id")));
    
    // NETWORKING Signal
    ADD_SIGNAL(MethodInfo("new_client_info", PropertyInfo(Variant::VECTOR3, "new_pos"),
            PropertyInfo(Variant::VECTOR3, "new_linear_vel"), 
            PropertyInfo(Variant::VECTOR3, "new_angular_vel"),
            PropertyInfo(Variant::INT, "laps")));
    
}

Player::Player() {
}

Player::~Player() {
}


void Player::_ready() {
    prev_position = get_position();
    state = Engine::get_singleton();
    player_camera = get_node<Camera3D>("./PlayerCamera");
    glide_timer = get_node<Timer>("./GlideTimer");
    if (!player_camera || !glide_timer) {
        return;
    }
    glide_timer->connect("timeout", Callable(this, "glide_timeout"));
    glide_timer->set_wait_time(DEFAULT_GLIDE_TIME);

    // Node3D* glider_ref = get_node<Node3D>("/root/World/Pausable/Level0/Glider");
    // if (glider_ref) {
    //     glider_ref->connect("glider_equip", Callable(this, "on_glider_equip"));
    // }

    gliderPlaceholder = get_node<MeshInstance3D>("./GliderPlaceholder");
    glider_gpos = gliderPlaceholder->get_global_position();
    gliderPlaceholder->set_visible(false);

    if (!Engine::get_singleton()->is_editor_hint()) {
        //play_collision_audio("sounds/background/background.wav", "background");
        play_collision_audio("sounds/synthwave/dark-synthwave.wav", "background");
        //res://sounds/synthwave/dark-synthwave.mp3
    }
}


void Player::_process(double delta) {
    if (state->is_editor_hint()) {
        return;
    }
    player_camera->set_rotation(Vector3(0, player_camera->get_rotation().y, 0)); // Keep only necessary rotations

    gliderPlaceholder->set_global_rotation(Vector3());
    Vector3 player_pos = get_global_position();
    player_pos.y +=0.5;
    gliderPlaceholder->set_global_position(player_pos);
    // Check if the player is grounded
    if (this->get_colliding_bodies().size() > 0) {
        is_grounded = true;
        emit_signal("is_grounded", true);

        is_gliding = false;
        can_glide = false; // Reset gliding permissions on landing
        emit_signal("glide_time_left", (float)DEFAULT_GLIDE_TIME);
        jumps_allowed = MAX_JUMPS;
        if (this->get_gravity_scale() != 0){
            stop_group_sound("wind");
            this->set_gravity_scale(normal_gravity_scale);
        }
        
    } else {
        emit_signal("is_grounded", false);

        is_grounded = false;
     
        // AIR CONTROL
        velocity *= air_control; 
        // AIR RESISTANCE
        Vector3 drag_force = -get_linear_velocity().normalized() * get_linear_velocity().length_squared() * air_resistance;
        drag_force.y=0;
        apply_central_force(drag_force);

        // Get the player's vertical velocity
        Vector3 vel = this->get_linear_velocity();
        // Start gliding only if player is falling and the jump button is held
        if (has_glider && can_glide && (vel.y <= 0)) {
            if (Input::get_singleton()->is_action_pressed("jump")){
                if (!glide_timer_started) {
                    glide_timer->start();
                    glide_timer_started = true;

                }
                is_gliding = true;
                if (this->get_gravity_scale() != 0){
                    this->set_gravity_scale(glide_gravity_scale);
                    if(get_tree()->get_nodes_in_group("wind").size()==0){
                        this->play_collision_audio("sounds/wind/wind.wav", "wind");
                    } else if (!mute_sfx){
                        resume_group_sound("wind");
                    }
                }
            } else {
                if (this->get_gravity_scale() != 0){
                    TypedArray<Node> wind = get_tree()->get_nodes_in_group("wind");
                    for(int i=0;i<wind.size();i++){
                        Variant var = wind[i];  
                        AudioStreamPlayer* asp = Object::cast_to<AudioStreamPlayer>(var);  
                        if(asp){
                            asp->stop();
                        } 
                    }

                    if (has_glider) {
                        this->set_gravity_scale(normal_gravity_scale);
                        glide_timer_started = false;
                        glide_timer->set_wait_time(DEFAULT_GLIDE_TIME);
                        glide_timer->stop();
                        can_glide = false;
                        
                        emit_signal("glide_time_left", (float)DEFAULT_GLIDE_TIME); // so that timer is reset when left space bar while gliding
                    }
                    pause_group_sound("wind");
                }
            }
            // Prevent gliding from reactivating mid-air
        } 
    }
    // show glider if the player is gliding
    gliderPlaceholder->set_visible(is_gliding);
    
    // Get the RayCast3D node named "Clip" from the player node
    RayCast3D* downRay = get_node<RayCast3D>("EdgeFront");
    if (downRay == nullptr) {
        UtilityFunctions::printerr("Failed to find RayCast3D node 'Down' under Player node.");
        return;
    }

    downRay->set_enabled(true);
    downRay->force_raycast_update();
    if (downRay->is_enabled() && downRay->is_colliding()) {
        // Get the object that was hit
        Object* collider = downRay->get_collider();
        if (collider) {
            Node* collider_node = cast_to<Node>(collider);
            if(collider_node){
                const String collider_name = collider_node->get_name();
                if(collider_name == "ProceduralPlane"){
                    double time = get_current_time_in_seconds();
                    if(time - last_bounce > 0.5){
                        play_collision_audio("sounds/bounce/bounce.ogg", "bounce");
                        last_bounce = time;
                    }
                } else if (collider_name == "PropulsionPlane") {
                    if(get_tree()->get_nodes_in_group("platform").size() == 0){
                        play_collision_audio("sounds/ice/ice.wav", "platform");
                    } else if (!mute_sfx){
                        resume_group_sound("platform");
                    }
                }
            }

            // Get the collision point and calculate distance
            Vector3 collision_point = downRay->get_collision_point();
            Vector3 origin = downRay->get_global_transform().origin;
            float distance = origin.distance_to(collision_point);
            Vector3 surface_normal = downRay->get_collision_normal();
            Vector3 up_direction = Vector3(0, 1, 0);
            float angle = up_direction.angle_to(surface_normal);
        }
    } else {
        pause_group_sound("platform");
    }

    // glide timer with GUI interaction
    if (glide_timer_started) {
        emit_signal("glide_time_left", (float)glide_timer->get_time_left());
    }

    // Emit player velocity using signal
    Vector3 curr_position = get_position();
    Vector3 speed_difference = (curr_position - prev_position);
    emit_signal("speed_updated", (float)speed_difference.length());
    prev_position = curr_position;
}



void Player::_physics_process() {
    Node* player_node = get_parent()->get_node<Node>("./Player"); 
    int laps = player_node->call("get_laps");
    emit_signal("new_client_info", get_position(), get_linear_velocity(), get_angular_velocity(), laps);

    // Movement!
    if (direction != Vector3{0, 0, 0}) {
        this->apply_central_impulse(velocity * 87 /*default mass*/);
    }

}

void Player::_input(const Ref<InputEvent>& event) {
    Input* i = Input::get_singleton();

    // if (camera_follow_mode) {
    // Get forward and right directions based on camera orientation
    Vector3 forward_direction = get_forward_direction(player_camera);
    Vector3 right_direction = get_right_direction(player_camera);

    // Calculate the input velocity based on movement input
    direction = forward_direction * (i->get_action_strength("up") - i->get_action_strength("down")) +
                    right_direction * (i->get_action_strength("right") - i->get_action_strength("left"));

    direction = direction.normalized();

    if (event->is_action_pressed("jump")) {
        if (is_grounded || jumps_allowed > 0) {
            // Perform a normal jump when grounded
            is_grounded = false;
            can_glide = true; // Allow gliding after the jump
            stop_group_sound("wind");
            this->set_gravity_scale(normal_gravity_scale);
            jumps_allowed--;
            this->apply_impulse(Vector3(0, 50, 0) * this->get_mass());
        }
    }

    // Sprint speed modifier
    if (event->is_action_pressed("sprint")) {
        velocity = direction.normalized() * (player_speed * 3);
    } else {
        velocity = direction.normalized() * player_speed;
    }

    if (Input::get_singleton()->is_action_pressed("sfx_mute")) {
        pause_group_sound("pausable");
    }

    if (Input::get_singleton()->is_action_pressed("volume_increase")) {
        TypedArray<Node> group = get_tree()->get_nodes_in_group("background");
        for(int i=0;i<group.size();i++){
            AudioStreamPlayer* asp = cast_to<AudioStreamPlayer>(group[i]);
            if(asp){
                asp->set_volume_db(asp->get_volume_db() + 1);
            }
        }
    }

    if (Input::get_singleton()->is_action_pressed("volume_decrease")) {
        TypedArray<Node> group = get_tree()->get_nodes_in_group("background");
        for(int i=0;i<group.size();i++){
            AudioStreamPlayer* asp = cast_to<AudioStreamPlayer>(group[i]);
            if(asp){
                asp->set_volume_db(asp->get_volume_db() - 1);
            }
        }
    }


}

Vector3 Player::get_forward_direction(Camera3D *camera) {
    // Get the camera's transform (rotation and position)
    Transform3D camera_transform = camera->get_transform();
    // Transform3D player_transform = this->get_transform();

    // Extract the forward direction from the basis (Z axis)
    Vector3 forward = -camera_transform.basis.get_column(2); // Forward is negative Z axis

    // Transform the camera's forward direction using the player's rotation (global transform)
    Basis player_basis = get_global_transform().basis;  // Get player's global basis
    forward = player_basis.xform(forward);  // Transform the forward direction to align with the player's rotation

    // Project onto the XZ plane to ignore any tilt (Y component)
    forward.y = 0;
    forward = forward.normalized();

    return forward;
}

Vector3 Player::get_right_direction(Camera3D *camera) {
    Vector3 forward = get_forward_direction(camera);
    return forward.cross(Vector3(0, 1, 0)).normalized(); // Right is perpendicular to forward and up
}


Vector3 Player::get_player_forward_direction() {
    // Get the camera's transform (rotation and position)
    Transform3D transform = get_global_transform();

    // Extract the forward direction from the basis (Z axis)
    Vector3 forward = -transform.basis.get_column(2); // Forward is negative Z axis

    // Project onto the XZ plane to ignore any tilt (Y component)
    forward.y = 0;
    forward = forward.normalized();

    return forward;
}

Vector3 Player::get_player_right_direction() {
    Vector3 forward = get_player_forward_direction();
    return forward.cross(Vector3(0, 1, 0)).normalized(); // Right is perpendicular to forward and up
}

void Player::play_collision_audio(const String& sound_path, const String& group_name) {
    AudioStreamPlayer *sound = memnew(AudioStreamPlayer);
    sound->add_to_group(group_name);
    if(group_name != "background")
        sound->add_to_group("pausable");

    sound->set_stream(ResourceLoader::get_singleton()->load(sound_path));
    add_child(sound);

    sound->connect("finished", Callable(sound, "queue_free"));
    if (!mute_sfx || group_name == "background") {
        sound->play();
    }
}

void Player::stop_group_sound(const String& group_name) {
    TypedArray<Node> group = get_tree()->get_nodes_in_group(group_name);
    for(int i=0;i<group.size();i++){
        Variant var = group[i];  
        AudioStreamPlayer* asp = cast_to<AudioStreamPlayer>(var);  
        if(asp){
            asp->remove_from_group(group_name);
            asp->stop();
        } 
    }
}

void Player::pause_group_sound(const String& group_name) {
    TypedArray<Node> group = get_tree()->get_nodes_in_group(group_name);
    for(int i=0;i<group.size();i++){
        Variant var = group[i];  
        AudioStreamPlayer* asp = cast_to<AudioStreamPlayer>(var);  
        if(asp){
            asp->set_stream_paused(true);
            mute_sfx = true;
        }
    }
}

void Player::resume_group_sound(const String& group_name) {
    TypedArray<Node> group = get_tree()->get_nodes_in_group(group_name);
    for(int i=0;i<group.size();i++){
        Variant var = group[i];  
        AudioStreamPlayer* asp = cast_to<AudioStreamPlayer>(var);  
        if(asp){
            asp->set_stream_paused(false);
            mute_sfx = false;
        } 
    }
}
void Player::glide_timeout() {
    can_glide = false;
    stop_group_sound("wind");
    this->set_gravity_scale(normal_gravity_scale);
    glide_timer_started = false;
    glide_timer->set_wait_time(DEFAULT_GLIDE_TIME);
    glide_timer->stop();

    // emit signal to the gui
}


void Player::on_glider_equip() {
    has_glider = true;
}
// Exported variable getters and setters

float Player::get_player_speed() const {
    return player_speed;
}

void Player::set_player_speed(const float new_player_speed) {
    player_speed = new_player_speed;
}

float Player::get_normal_gravity_scale() const {
    return normal_gravity_scale;
}

void Player::set_normal_gravity_scale(const float new_normal_gravity_scale) {
    normal_gravity_scale = new_normal_gravity_scale;
}

float Player::get_glide_gravity_scale() const {
    return glide_gravity_scale;
}

void Player::set_glide_gravity_scale(const float new_glide_gravity_scale) {
    glide_gravity_scale = new_glide_gravity_scale;
}

int Player::get_jumps_allowed() const {
    return MAX_JUMPS;
}

void Player::set_jumps_allowed(const int new_jumps_allowed) {
    MAX_JUMPS = new_jumps_allowed;
}

void Player::set_walkable_angle(const int new_walkable_angle) {
    walkable_angle = new_walkable_angle;
}

float Player::get_walkable_angle() const {
    return walkable_angle;
}

float Player::get_air_control() {
    return air_control;
}

void Player::set_air_control(float new_air_control) {
    air_control = new_air_control;
}


float Player::get_air_resistance() {
    return air_resistance;
}

void Player::set_air_resistance(float new_air_resistance) {
    air_resistance = new_air_resistance;
}

void Player::on_update_score(int value) {
    UtilityFunctions::print("player: on_update_score called with value", value);
    score += value;

    emit_signal("score_updated", score); // received by GUI
}

void Player::on_checkpoint_entered(int pos) {
    UtilityFunctions::print("did the checkpoint at ", pos);
    emit_signal("checkpoint_cleared", pos);
}

double Player::get_current_time_in_seconds() {
    // Get the current time point
    auto now = std::chrono::system_clock::now();
    
    // Get the duration since epoch in seconds as a double
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(now.time_since_epoch());
    
    return duration.count();  // Return the time in seconds as a double
}