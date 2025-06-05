#include "SettingsScreen.h"

using namespace godot;

void SettingsScreen::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_main_menu_button_pressed"), &SettingsScreen::on_main_menu_button_pressed);
    
    ADD_SIGNAL(MethodInfo("new_username", PropertyInfo(Variant::STRING, "new_username")));
    ADD_SIGNAL(MethodInfo("new_texture", PropertyInfo(Variant::INT, "new_texture")));
    ADD_SIGNAL(MethodInfo("load_start"));
}

void SettingsScreen::_ready() {
    mainMenuButton = get_node<Button>("./MainMenuButton");
    if (mainMenuButton) {
        mainMenuButton->connect("pressed", Callable(this, "on_main_menu_button_pressed"));
    } else {
        UtilityFunctions::printerr("Settings cant get reference to main menu button");
    }
}

void SettingsScreen::on_main_menu_button_pressed() {
    TextEdit* username = get_node<TextEdit>("./UsernameField");
    emit_signal("new_username", username->get_text());

    OptionButton* texture = get_node<OptionButton>("./BallTextureButton");
    emit_signal("new_texture", texture->get_selected());
    
    set_visible(false);
    get_parent()->get_node<Control>("StartScreen")->set_visible(true);

    emit_signal("load_start");
}
