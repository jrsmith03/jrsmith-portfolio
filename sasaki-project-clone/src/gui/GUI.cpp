#include "GUI.h"

using namespace godot;

void GUI::_bind_methods() {   
    ClassDB::bind_method(D_METHOD("on_score_updated", "new_score"), &GUI::on_score_updated);
    ClassDB::bind_method(D_METHOD("on_speed_updated", "new_speed"), &GUI::on_speed_updated);
    ClassDB::bind_method(D_METHOD("update_glide_timer_remaining", "time_left"), &GUI::update_glide_timer_remaining);
    ClassDB::bind_method(D_METHOD("update_checkpoint_time", "time_left"), &GUI::update_checkpoint_time);

    ClassDB::bind_method(D_METHOD("on_player_death"), &GUI::on_player_death);
    ClassDB::bind_method(D_METHOD("on_glider_equip"), &GUI::on_glider_equip);



}

GUI::GUI() {
}

GUI::~GUI() {
}

void GUI::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    // Node3D* glider_ref = get_node<Node3D>("/root/World/Pausable/Level0/Glider");
    // if (glider_ref) {
    //     glider_ref->connect("glider_equip", Callable(this, "on_glider_equip"));
    // }

    score_display = get_node<Control>("./Score");
    score_display->get_node<Label>("./name")->set_text("Score:");
    pause_menu = get_node<Control>("./PauseScreen");
    pause_menu->set_process_input(true);
    end_menu = get_node<Control>("./EndScreen");
    speedometer_hand = get_node<TextureRect>("./Speedometer/hand");

    
    glider_time_remaining = get_node<Control>("./Glider");
    if (glider_time_remaining) {
        glider_time_remaining->set_visible(false);
        glider_time_remaining->get_node<Label>("./name")->set_text("Glider:");
    }

    checkpoint_time_remaining = get_node<Control>("./CheckpointTime");
    if (checkpoint_time_remaining) {
        checkpoint_time_remaining->get_node<Label>("./value")->set_text("0");
    }

    TypedArray<Node3D> player_nodes = get_tree()->get_nodes_in_group("players");
    Node3D *player_ref = cast_to<Node3D>(player_nodes[0]);
    if (player_ref ) {
        player_ref->connect("score_updated", Callable(this, "on_score_updated"));

        player_ref->connect("glide_time_left", Callable(this, "update_glide_timer_remaining"));
        player_ref->connect("death", Callable(this, "on_player_death"));
        player_ref->connect("speed_updated", Callable(this, "on_speed_updated"));

    } 
}


void GUI::_process(double delta) {
    if (!fker_found) {
        level_ref = get_node<Node3D>("/root/World/SingleplayerLevel");
        if (level_ref) {
            fker_found = true;
            level_ref->connect("checkpoint_time_left", Callable(this, "update_checkpoint_time"));
        }
    }
}
void GUI::_input(const Ref<InputEvent>& event) {
    if (event->is_action_pressed("pause")) { // Trigger when pause key is pressed
        if (get_tree()->is_paused()) {
            // Unpause
            UtilityFunctions::printerr("UNPAUSING");
            get_tree()->set_pause(false);
            pause_menu->set_visible(false); // Hide the pause menu
            Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);
        } else {
            // Pause
            UtilityFunctions::printerr("PAUSING");
            get_tree()->set_pause(true);
            pause_menu->set_visible(true); // Show the pause menu
            Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_VISIBLE);

        }
    }

    // Check for quit action only when paused
    if (get_tree()->is_paused() && event->is_action_pressed("quit")) {
        UtilityFunctions::printerr("QUITTING");
        get_tree()->quit();
    }
}




void GUI::on_score_updated(int new_score) {
    score_display->get_node<Label>("./value")->set_text(String::num(new_score));
}

void GUI::on_speed_updated(float new_speed) {
    // we want to map the speed between 0 degree and 264 degree (taken from speedometer sprites)
    // we will assume the max speed of 8
    float hand_rotation_degrees = remap_range(new_speed, 0.0, 8.0, 0.0, 264.0);
    speedometer_hand->set_rotation_degrees(hand_rotation_degrees);
}

void GUI::update_glide_timer_remaining(float time_left) {
    // assuming default time is 2.0f
    // therefore: width of current rect box = 
    // make a timer typed label
    float nwidth = remap_range(time_left, 0.0f, 2.0f, 0.0f, 200.0f);
    Vector2 size = glider_time_remaining->get_node<ColorRect>("./current")->get_size();
    size.x = nwidth;
    glider_time_remaining->get_node<ColorRect>("./current")->set_size(size);

}

void GUI::update_checkpoint_time(float time_left) {
    UtilityFunctions::print("I GOT A SIGNAL AND I AM SO HAPPPPPYYYYYY");
    // assuming default time is 2.0f
    // therefore: width of current rect box = 
    // make a timer typed label
    // float nwidth = remap_range(time_left, 0.0f, 2.0f, 0.0f, 200.0f);
    // Vector2 size = glider_time_remaining->get_node<ColorRect>("./current")->get_size();
    // size.x = nwidth;
    // glider_time_remaining->get_node<ColorRect>("./current")->set_size(size);
    float rounded = std::round(time_left * 100.0f)/100.0f;
    checkpoint_time_remaining->get_node<Label>("./value")->set_text(String::num_real(rounded, 2));


}

void GUI::on_player_death() {
    // end_menu->set_visible(true);
    // Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_VISIBLE);
    // get_tree()->set_pause(true);
    
    // if (Input::get_singleton()->is_action_pressed("quit")) {
    //     get_tree()->quit();
    // }
}

void GUI::on_glider_equip() {
    glider_time_remaining->set_visible(true);
}

float GUI::remap_range(float val, float srcLo, float srcHi, float destLo, float destHi) {
    return(val - srcLo) / (srcHi - srcLo) * (destHi - destLo) + destLo;
}