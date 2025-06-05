#include "LobbyScreen.h"

using namespace godot;

void LobbyScreen::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_exit_button_pressed"), &LobbyScreen::on_exit_button_pressed);
    ClassDB::bind_method(D_METHOD("on_start_button_pressed"), &LobbyScreen::on_start_button_pressed);
    
}

void LobbyScreen::_ready() {
    exitButton = get_node<Button>("./ExitButton");
    if (exitButton) {
        exitButton->connect("pressed", Callable(this, "on_exit_button_pressed"));
    } else {
        UtilityFunctions::printerr("lobby cant get reference to exit button");
    }

    startButton = get_node<Button>("./StartButton");
    if (startButton) {
        startButton->connect("pressed", Callable(this, "on_start_button_pressed"));
    } else {
        UtilityFunctions::printerr("multiplayer cant get reference to start button");
    }
}

void LobbyScreen::on_exit_button_pressed() {
    // emit_signal("load_multiplayer");
}

void LobbyScreen::on_start_button_pressed() {
    get_tree()->change_scene_to_file("res://World.tscn");
}
