#include "StartScreen.h"


using namespace godot;

void StartScreen::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_solo_button_pressed"), &StartScreen::on_solo_button_pressed);
    ClassDB::bind_method(D_METHOD("on_multiplayer_button_pressed"), &StartScreen::on_multiplayer_button_pressed);
    ClassDB::bind_method(D_METHOD("on_settings_button_pressed"), &StartScreen::on_settings_button_pressed);
    ClassDB::bind_method(D_METHOD("on_quit_button_pressed"), &StartScreen::on_quit_button_pressed);
    ClassDB::bind_method(D_METHOD("on_server_button_pressed"), &StartScreen::on_server_button_pressed);
    
    ADD_SIGNAL(MethodInfo("load_settings"));
    ADD_SIGNAL(MethodInfo("load_multiplayer"));
    ADD_SIGNAL(MethodInfo("load_singleplayer"));
    ADD_SIGNAL(MethodInfo("load_multiplayer_screen"));


}

void StartScreen::_ready() {
    settingsButton = get_node<Button>("./Buttons/SettingsButton");
    if (settingsButton) {
        settingsButton->connect("pressed", Callable(this, "on_settings_button_pressed"));
    } else {
        UtilityFunctions::printerr("Start cant get reference to settings button");
    }
    
    quitButton = get_node<Button>("./Buttons/QuitButton");
    if (quitButton) {
        quitButton->connect("pressed", Callable(this, "on_quit_button_pressed"));
    } else {
        UtilityFunctions::printerr("Start cant get reference to quit button");
    }

    soloButton = get_node<Button>("./Buttons/SoloButton");
    if (soloButton) {
        soloButton->connect("pressed", Callable(this, "on_solo_button_pressed"));
    } else {
        UtilityFunctions::printerr("Start cant get reference to solo button");
    }

    multiplayerButton = get_node<Button>("./Buttons/MultiplayerButton");
    if (multiplayerButton) {
        multiplayerButton->connect("pressed", Callable(this, "on_multiplayer_button_pressed"));
    } else {
        UtilityFunctions::printerr("Start cant get reference to multiplayer button");
    }
}

void StartScreen::on_solo_button_pressed() {
    set_visible(false);
    ParallaxBackground* pb = get_node<ParallaxBackground>("LavaPB");
    if (!pb) {
        UtilityFunctions::print("We are all truly fucked.");
    }
    pb->queue_free();

    emit_signal("load_singleplayer");   // let somebody know we're in singleplayer

}

void StartScreen::on_multiplayer_button_pressed() {
    /*
        - Ask NetMain if I am server or client:
        -  If server: emit signal (set server mode)
        -  If client: emit signal (set client mode)
    */
    emit_signal("load_multiplayer_screen");
}

void StartScreen::on_settings_button_pressed() {
    set_visible(false);
    get_parent()->get_node<Control>("SettingsScreen")->set_visible(true);
}

void StartScreen::on_quit_button_pressed() {
    get_tree()->quit();
}

void StartScreen::on_server_button_pressed() {
    emit_signal("set_server_mode", true);
    emit_signal("load_lobby");
}


