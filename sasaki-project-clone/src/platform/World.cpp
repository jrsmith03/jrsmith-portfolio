#include "World.h"



#include <cstdlib>

using namespace godot;

void World::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_multiplayer_level"),
        &World::set_multiplayer_level);
    ClassDB::bind_method(D_METHOD("set_singleplayer_level"),
        &World::set_singleplayer_level);

    ClassDB::bind_method(D_METHOD("set_lobby_level"),
        &World::set_lobby_level);
    
    ClassDB::bind_method(D_METHOD("set_multiplayer_screen"),
        &World::set_multiplayer_screen);
    
    ADD_SIGNAL(MethodInfo("is_singleplayer", PropertyInfo(Variant::BOOL, "state")));
}

World::World() {
}

World::~World() {
}

void World::_ready() {
    menu = get_node<Control>("StartScreen");
    Node* NetMain = get_node<Node>("NetMain");
    if (menu) {
        menu->connect("load_singleplayer", Callable(this, "set_singleplayer_level"));
        NetMain->connect("load_multiplayer", Callable(this, "set_multiplayer_level")); 
        menu->connect("load_multiplayer_screen", Callable(this, "set_multiplayer_screen"));
    } else {
        UtilityFunctions::printerr("Unable to connect to the menu, so nothing will work!");
    }

    Control* multiplayerScreen = get_node<Control>("MultiplayerScreen");
    if (multiplayerScreen) {
        multiplayerScreen->connect("load_lobby", Callable(this, "set_lobby_level"));
    }

    resultsScreen = get_node<Control>("ResultsScreen");
    if (resultsScreen) {
        NetMain->connect("show_results",Callable(resultsScreen, "show_results"));
    } else {
        UtilityFunctions::printerr("Failed to connect show_results signal.");
    }

}
 
void World::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    emit_signal("is_singleplayer", is_singleplayer);
}

void World::set_lobby_level() {
    set_environment();
    set_level("res://gd-level/LobbyScene.tscn");
}

void World::set_multiplayer_screen() {
    Control* start_screen = get_node<Control>("StartScreen");
    Control* multiplayer_screen = get_node<Control>("MultiplayerScreen");
    if (start_screen) {
        start_screen->set_visible(false);
        if (multiplayer_screen) {
            multiplayer_screen->set_visible(true);
        } else {
            UtilityFunctions::printerr("world couldn't find multiplayer screen");
        }
    } else {
        UtilityFunctions::printerr("world couldn't find start screen");
    }
}


void World::set_multiplayer_level() {
    // lobby scene will be there - queue free it
    Node3D* lobby = get_node<Node3D>("LobbyScene");
    if (lobby) {
        lobby->set_visible(false);
    }
    set_level("res://gd-level/levels/MultiplayerLevel.tscn");
}

void World::set_singleplayer_level() {
    is_singleplayer = true;
    set_environment();
    set_level("res://gd-level/levels/SingleplayerLevel.tscn");
}

void World::set_environment() {
    Ref<PackedScene> player_ref = ResourceLoader::get_singleton()->load("res://gd-player/Player.tscn");
    Ref<PackedScene> environment_ref = ResourceLoader::get_singleton()->load("res://gd-level/environment.tscn");

    if (!environment_ref.is_valid() && !player_ref.is_valid()) {
        UtilityFunctions::printerr("Unable to load in the Singleplayer level resource.");
        return;
    }
    Node* environment = environment_ref->instantiate();
    Node* player_ins = player_ref->instantiate();
    RigidBody3D* player = static_cast<RigidBody3D*>(player_ins);
    if (!environment || !player) {
        UtilityFunctions::printerr("Unable to instantiate the Singleplayer level.");
        return;
    }
    player->set_global_position(Vector3(0, 200, 0));
    add_child(player);
    add_child(environment);
    menu->set_visible(false);
}

int World::set_level(String level_path) {
    Ref<PackedScene> level_ref = ResourceLoader::get_singleton()->load(level_path);
    if (!level_ref.is_valid()) {
        UtilityFunctions::printerr("Unable to find level scene resource.");
        return -1;
    }

    Node* my_level = level_ref->instantiate();
    if (!my_level) {
        UtilityFunctions::printerr("Unable to instantiate level.");
        return -1;
    }
    add_child(my_level);
    return 0;
}