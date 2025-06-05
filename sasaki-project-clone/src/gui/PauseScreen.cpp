#include "PauseScreen.h"

using namespace godot;

void PauseScreen::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_resume_button_pressed"), &PauseScreen::on_resume_button_pressed);
    ClassDB::bind_method(D_METHOD("on_quit_button_pressed"), &PauseScreen::on_quit_button_pressed);

}

PauseScreen::PauseScreen() {

}
PauseScreen::~PauseScreen() {

}

void PauseScreen::_ready() {
    resumeButton = get_node<Button>("./VBox/ResumeButton");
    if (resumeButton) {
        resumeButton->connect("pressed", Callable(this, "on_resume_button_pressed"));
    } else {
        UtilityFunctions::printerr("Pause cant get reference to resume button");
    }

    quitButton = get_node<Button>("./VBox/QuitButton");
    if (quitButton) {
        quitButton->connect("pressed", Callable(this, "on_quit_button_pressed"));
    } else {
        UtilityFunctions::printerr("Pause cant get reference to quit button");
    }
    
    Input* input = Input::get_singleton();
}

void PauseScreen::on_resume_button_pressed() {
    // check if tree is paused, if yes then unpause. Else do nothing
    if (get_tree()->is_paused()) {
        get_tree()->set_pause(false);
        set_visible(false);
        Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);
    } 
}

void PauseScreen::on_quit_button_pressed() {
    get_tree()->quit();
}

void PauseScreen::_input(const Ref<InputEvent>& event) {

}
