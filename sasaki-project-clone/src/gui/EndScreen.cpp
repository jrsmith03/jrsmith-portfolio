#include "EndScreen.h"

using namespace godot;

void EndScreen::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_restart_button_pressed"), &EndScreen::on_restart_button_pressed);
    ClassDB::bind_method(D_METHOD("on_quit_button_pressed"), &EndScreen::on_quit_button_pressed);

}

EndScreen::EndScreen() {

}
EndScreen::~EndScreen() {

}

void EndScreen::_ready() {
    restartButton = get_node<Button>("./RestartButton");
    if (restartButton) {
        restartButton->connect("pressed", Callable(this, "on_restart_button_pressed"));
    } else {
        UtilityFunctions::printerr("End Screen cant get reference to restart button");
    }

    quitButton = get_node<Button>("./QuitButton");
    if (quitButton) {
        quitButton->connect("pressed", Callable(this, "on_quit_button_pressed"));
    } else {
        UtilityFunctions::printerr("End screen cant get reference to quit button");
    }
    
    Input* input = Input::get_singleton();
}

void EndScreen::on_restart_button_pressed() {
    // check if tree is paused, if yes then unpause. Else do nothing
    get_tree()->reload_current_scene();
    
    get_tree()->set_pause(false);
}

void EndScreen::on_quit_button_pressed() {
    get_tree()->quit();
}

void EndScreen::_input(const Ref<InputEvent>& event) {

}