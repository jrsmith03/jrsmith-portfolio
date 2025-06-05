#include "MainMenu.h"

using namespace godot;

void MainMenu::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_server_update", "new_server"), &MainMenu::on_server_update);
    ClassDB::bind_method(D_METHOD("on_protocol_update", "new_protocol"), &MainMenu::on_protocol_update);
    ClassDB::bind_method(D_METHOD("on_username_update", "new_username"), &MainMenu::on_username_update);
    ClassDB::bind_method(D_METHOD("on_texture_update", "new_texture"), &MainMenu::on_texture_update);
    ClassDB::bind_method(D_METHOD("on_level_update", "new_level"), &MainMenu::on_level_update);

    ClassDB::bind_method(D_METHOD("on_load_start"), &MainMenu::on_load_start);
    ClassDB::bind_method(D_METHOD("on_load_settings"), &MainMenu::on_load_settings);
    ClassDB::bind_method(D_METHOD("on_load_multiplayer"), &MainMenu::on_load_multiplayer);
    ClassDB::bind_method(D_METHOD("on_load_lobby"), &MainMenu::on_load_lobby);
}

void MainMenu::_ready() {
    // MultiplayerScreen conf
    server_ip = "172.69.69.69:42069";
    selected_protocol = 0;

    // SettingsScreen conf
    username = "Player";
    selected_texture = 0;

    // Unimplemented
    selected_level = 0;
}

// MultiplayerScreen conf
void MainMenu::on_server_update(String new_server){
    server_ip = new_server;
}

void MainMenu::on_protocol_update(uint8_t new_protocol){
    selected_protocol = new_protocol;
}

// SettingsScreen conf
void MainMenu::on_username_update(String new_username){
    username = new_username;
}

void MainMenu::on_texture_update(uint8_t new_texture){
    selected_texture = new_texture;
}

// Unimplemented
void MainMenu::on_level_update(uint8_t new_level){
    selected_level = new_level;
}

void MainMenu::on_load_start() {
    StartScreen* start = get_node<StartScreen>("./StartScreen");
    close_menus();
    start->set_visible(true);
}

void MainMenu::on_load_settings() {
    SettingsScreen* settings = get_node<SettingsScreen>("./SettingsScreen");
    close_menus();
    settings->set_visible(true);
}

void MainMenu::on_load_multiplayer() {
    MultiplayerScreen* multiplayer = get_node<MultiplayerScreen>("./MultiplayerScreen");
    close_menus();
    multiplayer->set_visible(true);
}

void MainMenu::on_load_lobby() {
    LobbyScreen* lobby = get_node<LobbyScreen>("./LobbyScreen");
    close_menus();
    lobby->set_visible(true);
}

void MainMenu::close_menus() {
    TypedArray<Node> children = get_children();
    while (!children.is_empty()) {
        Variant var = children.pop_back();
        Control *node = Object::cast_to<Control>(var);
        if (node)
            node->set_visible(false);
    }
}
