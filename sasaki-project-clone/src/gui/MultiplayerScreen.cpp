#include "MultiplayerScreen.h"

using namespace godot;

void MultiplayerScreen::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_main_menu_button_pressed"), &MultiplayerScreen::on_main_menu_button_pressed);
    ClassDB::bind_method(D_METHOD("on_join_server_button_pressed"), &MultiplayerScreen::on_join_server_button_pressed);
    ClassDB::bind_method(D_METHOD("on_start_server_button_pressed"), &MultiplayerScreen::on_start_server_button_pressed);
    
    ADD_SIGNAL(MethodInfo("server_info", PropertyInfo(Variant::STRING, "ipaddress")));
    ADD_SIGNAL(MethodInfo("protocol_update", PropertyInfo(Variant::INT, "new_protocol")));
    ADD_SIGNAL(MethodInfo("load_lobby"));
    ADD_SIGNAL(MethodInfo("load_start"));
    ADD_SIGNAL(MethodInfo("set_server_mode", PropertyInfo(Variant::BOOL, "state")));
    ADD_SIGNAL(MethodInfo("set_client_mode", PropertyInfo(Variant::BOOL, "state")));
    ADD_SIGNAL(MethodInfo("new_username", PropertyInfo(Variant::STRING, "new_username")));

}

void MultiplayerScreen::_ready() {
    mainMenuButton = get_node<Button>("./MainMenuButton");
    if (mainMenuButton) {
        mainMenuButton->connect("pressed", Callable(this, "on_main_menu_button_pressed"));
    } else {
        UtilityFunctions::printerr("multiplayer cant get reference to main menu button");
    }
    
    startServerButton = get_node<Button>("./StartServerButton");
    if (startServerButton) {
        startServerButton->connect("pressed", Callable(this, "on_start_server_button_pressed"));
    } else {
        UtilityFunctions::printerr("multiplayer cant get reference to start server button");
    }
    joinServerButton = get_node<Button>("./JoinServerButton");
    if (joinServerButton) {
        joinServerButton->connect("pressed", Callable(this, "on_join_server_button_pressed"));
    } else {
        UtilityFunctions::printerr("multiplayer cant get reference to join server button");
    }

    serverField = get_node<TextEdit>("./ServerField");
    if (serverField) {
        PackedStringArray local_addresses = IP::get_singleton()->get_local_addresses();
        serverField->set_text("127.0.0.1");
    } else {
        UtilityFunctions::printerr("multiplayer cant get reference to server field object");
    }
}

void MultiplayerScreen::on_main_menu_button_pressed() {
    signal_main();
    set_visible(false);
    get_parent()->get_node<Control>("StartScreen")->set_visible(true);

}



void MultiplayerScreen::signal_main(){
}


void MultiplayerScreen::on_start_server_button_pressed() {
    signal_main();
    Control* startScreen = get_parent()->get_node<Control>("StartScreen");
    TextEdit* name = get_node<TextEdit>("./NameField");

    set_visible(false);
    emit_signal("set_server_mode", true);
    emit_signal("load_lobby");
    emit_signal("new_username", name->get_text());

}



void MultiplayerScreen::on_join_server_button_pressed() {
    TextEdit* server = get_node<TextEdit>("./ServerField");
    TextEdit* name = get_node<TextEdit>("./NameField");

    emit_signal("server_info", server->get_text());
    emit_signal("server_info", server->get_text());

    Control* startScreen = get_parent()->get_node<Control>("StartScreen");
    set_visible(false);
    emit_signal("set_client_mode", true);
    emit_signal("load_lobby");
    emit_signal("new_username", name->get_text());
}

