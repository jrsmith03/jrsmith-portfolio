#include "NetMain.h"

using namespace godot;

void NetMain::_bind_methods() {
    // connect to the signal to figure out if we are, in fact, the client or Server
    ClassDB::bind_method(D_METHOD("on_set_server", "state"),
        &NetMain::on_set_server);
    ClassDB::bind_method(D_METHOD("on_set_client", "state"),
        &NetMain::on_set_client);
    ClassDB::bind_method(D_METHOD("update_player_info", "pos", "linear_vel", "angular_vel", 
        "is_ready", "uid", "lap_count", "name"), &NetMain::update_player_info);

    ClassDB::bind_method(D_METHOD("add_player", "pid", "cur_roster"),
        &NetMain::add_player);
    ClassDB::bind_method(D_METHOD("update_other_rosters", "to_add"),
        &NetMain::update_other_rosters);

    ClassDB::bind_method(D_METHOD("on_peer_connected", "id"),
        &NetMain::on_peer_connected);

    ClassDB::bind_method(D_METHOD("on_peer_disconnected", "id"),
        &NetMain::on_peer_disconnected);

    ClassDB::bind_method(D_METHOD("on_new_client_info", "new_pos", "new_linear_vel",
        "new_angular_vel", "new_laps"), &NetMain::on_new_client_info); 

    ClassDB::bind_method(D_METHOD("on_new_username_created", "new_username"),
        &NetMain::on_new_username_created);

    ClassDB::bind_method(D_METHOD("on_new_texture_created", "new_texture"),
        &NetMain::on_new_texture_created);

    ClassDB::bind_method(D_METHOD("on_player_ready_pressed"),
        &NetMain::on_player_ready_pressed);

    ClassDB::bind_method(D_METHOD("start_game"),
        &NetMain::start_game);

    ClassDB::bind_method(D_METHOD("on_server_info"),
        &NetMain::on_server_info);

    ADD_SIGNAL(MethodInfo("load_multiplayer"));

    ClassDB::bind_method(D_METHOD("emit_show_results_signal", "results_text"),
        &NetMain::emit_show_results_signal);
    ADD_SIGNAL(MethodInfo("show_results", PropertyInfo(Variant::STRING, "results_text")));

    ClassDB::bind_method(D_METHOD("notify_race_finished", "client_id"),
        &NetMain::notify_race_finished);

    ClassDB::bind_method(D_METHOD("remove_player", "pid"),
        &NetMain::remove_player);

    ClassDB::bind_method(D_METHOD("get_lerp_weight"),
         &NetMain::get_lerp_weight);
    ClassDB::bind_method(D_METHOD("set_lerp_weight", "new_lerp_weight"), 
        &NetMain::set_lerp_weight);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lerp_weight"), 
        "set_lerp_weight", "get_lerp_weight");
    ClassDB::bind_method(D_METHOD("get_lerp_threshold"),
         &NetMain::get_lerp_threshold);
    ClassDB::bind_method(D_METHOD("set_lerp_threshold", "new_lerp_threshold"), 
        &NetMain::set_lerp_threshold);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lerp_threshold"), 
        "set_lerp_threshold", "get_lerp_threshold");

    ClassDB::bind_method(D_METHOD("get_artificial_lag"),
         &NetMain::get_artifical_lag);
    ClassDB::bind_method(D_METHOD("set_artifical_lag", "new_artifical_lag"), 
        &NetMain::set_artificial_lag);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "artifical_lag"), 
        "set_artifical_lag", "get_artificial_lag");
    ClassDB::bind_method(D_METHOD("disable_net"), &NetMain::disable_net);
    ADD_SIGNAL(MethodInfo("load_start"));
    
}

NetMain::NetMain() {
}

NetMain::~NetMain() {
}

void NetMain::_ready() {
	Dictionary rpc_dict_2;
    Node* multiplayer_screen = get_parent()->get_node<Node>("./MultiplayerScreen");
    get_parent()->connect("is_singleplayer", Callable(this, "disable_net"));

    multiplayer_screen->connect("set_server_mode", Callable(this, "on_set_server"));
    multiplayer_screen->connect("set_client_mode", Callable(this, "on_set_client"));
    multiplayer_screen->connect("server_info", Callable(this, "on_server_info"));
    multiplayer_screen->connect("new_username", Callable(this, "on_new_username_created"));
    
    net_api = get_tree()->get_multiplayer();
    net_api->connect("connected_to_server", Callable(this, "on_connected_to_server"));

    Node* settings = get_parent()->get_node<Node>("./SettingsScreen");

    // for info coming from settings screen
    settings->connect("new_username", Callable(this, "on_new_username_created"));
    settings->connect("new_texture", Callable(this, "on_new_texture_created"));

    // Our default RPC config.
    // Authority will call only on client. ANY_PEER will call on client & server.
    rpc_dict["rpc_mode"] = MultiplayerAPI::RPC_MODE_ANY_PEER;
    rpc_dict["transfer_mode"] = MultiplayerPeer::TRANSFER_MODE_RELIABLE;
    rpc_dict_2["rpc_mode"] = MultiplayerAPI::RPC_MODE_AUTHORITY;
    rpc_dict_2["transfer_mode"] = MultiplayerPeer::TRANSFER_MODE_RELIABLE;
    net_api->connect("peer_connected", Callable(this, "on_peer_connected"));
    net_api->connect("peer_disconnected", Callable(this, "on_peer_disconnected"));

    rpc_config("add_player", rpc_dict);
    rpc_config("update_other_rosters", rpc_dict);
    rpc_config("update_player_info", rpc_dict);
    rpc_config("emit_show_results_signal", rpc_dict);
 
    rpc_config("start_game", rpc_dict_2);

    rpc_config("notify_race_finished", rpc_dict);
    rpc_config("remove_player", rpc_dict);


}

void NetMain::on_server_info(String new_ip) {
    ip_address = new_ip;
}

void NetMain::disable_net(bool shutup) {
    should_disable_net = true;
}


void NetMain::on_new_username_created(String new_username) {
    roster[uid].name = new_username;
}

void NetMain::on_new_texture_created(int new_texture) {
    texture_id = new_texture;
}

int return_delay = 0;
void NetMain::_process(double delta) {

    if (Engine::get_singleton()->is_editor_hint() || should_disable_net) {
        return;
    }
    if (!player_attached) {
        Node* this_player = get_parent()->get_node<Node>("./Player");
        if (this_player) {
            player_attached = true;
            this_player->connect("new_client_info", Callable(this, "on_new_client_info"));
        } 
    }
    if (Node3D* lobby = get_parent()->get_node<Node3D>("LobbyScene")) {
        if (!lobby_signal_connected) {
            lobby->connect("player_ready_pressed", Callable(this, "on_player_ready_pressed"));
            lobby_signal_connected = true;
        }
    }
    int ready_count = 0;
    for (const auto &pair : roster) {
        if (pair.second.ptr != nullptr) {
            // Dead Reckoning
            pair.second.ptr->set_linear_velocity(pair.second.linear_vel);
            pair.second.ptr->set_angular_velocity(pair.second.angular_vel);
        
            // Interpolate position if there's a noticeable discrepancy
            Vector3 current_position = pair.second.ptr->get_global_position();

            if (current_position.distance_to(pair.second.pos) > lerp_threshold) {
                Vector3 corrected_position = current_position.lerp(pair.second.pos, lerp_weight);
                pair.second.ptr->set_global_position(corrected_position);
            }
        }
        if (pair.second.is_ready) {
            ready_count++;
        }
    }

    if (!game_started && roster.size() > 1 && !game_in_session && uid == SERVER && (ready_count == (roster.size() - 1))) {
        // Tell all the clients to start the game.
        game_in_session = true;
        emit_signal("load_multiplayer");
        game_started = true;
        for (const auto &pair : roster) {
            rpc_id(pair.first, "start_game");       
        }
    }

    if (!game_in_session && game_won) {
        if (return_delay == 200) {
            Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_VISIBLE);
            get_tree()->change_scene_to_file("res://World.tscn");
        }
        return_delay++;
    }
}

String NetMain::debugString(bool who){
    return who ? "[Server]" : "[Client]";
}

void NetMain::on_set_server(bool state) {
    // If we're a server, set the state.
    is_server = state;
    roster.clear();
    // set the ip_address to the ip_address of the interface on this machine
    Ref<ENetMultiplayerPeer> server;
    server.instantiate();
    server.ptr()->create_server(SERVER_PORT, 4, 0, 5, 5);
    net_api.ptr()->set_multiplayer_peer(server);
    uid = SERVER;
    ClientPlayer entry;
    entry.pos = Vector3(0, 200, 0);
    entry.ptr = nullptr;
    entry.lap_count = 0;
    roster[1].is_ready = false;
}

void NetMain::on_set_client(bool state) {
    is_client = state;
    Ref<ENetMultiplayerPeer> new_client;
    new_client.instantiate();
    new_client.ptr()->create_client(ip_address, SERVER_PORT);
    net_api.ptr()->set_multiplayer_peer(new_client);
}


void NetMain::on_peer_connected(int id) {
    // only server will call add_player, once clients connect to it
    if (uid == SERVER) {
        Array cur_roster_id;
        for (auto &player_par : roster) {
            cur_roster_id.append(player_par.first);
        }
        rpc_id(id, "add_player", id, cur_roster_id);
    }
}


// This RPC is executed on the client. We are setting the current roster equal to the most recent roster from server.
void NetMain::add_player(int pid, Array current_roster_ids) {
    roster.clear();
    uid = pid;

    // Add the players inside the roster to the current scene tree.
    for (int i = 0; i < current_roster_ids.size(); i++) {
        Ref<PackedScene> rplayer_ref = ResourceLoader::get_singleton()->load("res://gd-player/RemotePlayer.tscn");
        Node* rplayer_ins = rplayer_ref->instantiate();
        RigidBody3D* player = static_cast<RigidBody3D*>(rplayer_ins);
        // Keep the id, but update the pointer in this address space for the appropriate ref.
        ClientPlayer entry;
        entry.pos = Vector3(0, 200, 0);
        entry.linear_vel = Vector3(0, 0, 0);
        entry.angular_vel = Vector3(0, 0, 0);
        entry.name = "sasakiPlayer";
        entry.lap_count = 0;
        entry.ptr = player;
        entry.ptr->set_position(Vector3(0, 0, 0));
        roster[current_roster_ids[i]] = entry;

        add_child(player);
        
        // Update the rosters and scene trees of all the other players with the instance of the new player
        rpc_id(current_roster_ids[i], "update_other_rosters", pid);
    }

}

void NetMain::on_peer_disconnected(int id) {
    roster[id].ptr->queue_free();
    if (uid != SERVER && id == SERVER) {
        // RETURN CLIENTS TO MAIN SCREEN
        Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_VISIBLE);
        get_tree()->change_scene_to_file("res://World.tscn");
    }
    roster.erase(id);
}

// Callback emitted by each client whever their position changes. Update the rosters of all clients.
int lag = 0;
void NetMain::on_new_client_info(Vector3 new_pos, Vector3 new_linear_vel, Vector3 new_angular_vel, int new_laps) {
    roster[uid].pos = new_pos;
    roster[uid].linear_vel = new_linear_vel;
    roster[uid].angular_vel = new_angular_vel;
    roster[uid].lap_count = new_laps;
    if (artifical_lag == -1 || ++lag == artifical_lag) {
        rpc("update_player_info", new_pos, new_linear_vel, new_angular_vel, 
                roster[uid].is_ready, uid, new_laps, roster[uid].name);
        if (artifical_lag != -1) {
            lag = 0;
        }
    } 
    if (new_laps >= MAX_LAPS) {
        if (uid != SERVER) {
            rpc_id(SERVER, "notify_race_finished", uid);
        } else {
            end_game();
        }
    }
}

void NetMain::notify_race_finished(int client_id) {
    end_game();
}


// RPC to add new client to all other clients
void NetMain::update_other_rosters(int to_add) {
    // First, add to scene tree
    Ref<PackedScene> rplayer_ref = ResourceLoader::get_singleton()->load("res://gd-player/RemotePlayer.tscn");
    Node* rplayer_ins = rplayer_ref->instantiate();
    RigidBody3D* player = static_cast<RigidBody3D*>(rplayer_ins);
    ClientPlayer entry;
    entry.pos = Vector3(0, 200, 0);
    entry.linear_vel = Vector3(0, 0, 0);
    entry.angular_vel = Vector3(0, 0, 0);
    entry.ptr = player;
    entry.is_ready = false;
    entry.lap_count = 0;
    // Now, add to the current roster.
    roster[to_add] = entry;
    add_child(player);
}

// RPC to set the new position of the caller on all clients
void NetMain::update_player_info(Vector3 pos, Vector3 linear_vel, Vector3 angular_vel, 
                    bool is_ready, int caller_id, int lap_count, String name) {
                        
    roster[caller_id].pos = pos;
    roster[caller_id].linear_vel = linear_vel;
    roster[caller_id].angular_vel = angular_vel;
    roster[caller_id].lap_count = lap_count;
    roster[caller_id].name = name;
    if (is_ready && !roster[caller_id].is_ready) {
        roster[caller_id].is_ready = is_ready;
    }
}

void NetMain::on_player_ready_pressed() {
    roster[uid].is_ready = true;
    rpc("update_player_info", Vector3(0,0,0), Vector3(0,0,0), Vector3(0,0,0), true, uid, 0, roster[uid].name);
}


void NetMain::start_game() {
    emit_signal("load_multiplayer");
}

void NetMain::end_game() {
    if(game_in_session){
        // Build the race results string once here
        String results_summary = "Race Finished! Here are the results:\n";
        for (const auto& pair : roster) {
            int player_id = pair.first;
            int lap_count = pair.second.lap_count;
            results_summary += "Player " + pair.second.name + ": " + String::num(lap_count) + " laps\n";
        }
        
        // Use rpc to call emit_show_results_signal across all clients
        emit_show_results_signal(results_summary);
        rpc("emit_show_results_signal", results_summary);

        game_in_session = false;
        game_won = true;
    }

}


void NetMain::emit_show_results_signal(const String &results_text) {
    emit_signal("show_results", results_text);

}

void NetMain::remove_player(int pid) {
    roster[pid].ptr->queue_free();
}

float NetMain::get_lerp_weight() {
    return lerp_weight;
}

void NetMain::set_lerp_weight(float new_lerp_weight) {
    lerp_weight = new_lerp_weight;
}

float NetMain::get_lerp_threshold() {
    return lerp_threshold;
}
void NetMain::set_lerp_threshold(float new_lerp_threshold){
    lerp_threshold = new_lerp_threshold;
}

int NetMain::get_artifical_lag() {
    return artifical_lag;
}
void NetMain::set_artificial_lag(int new_artifical_lag){
    artifical_lag = new_artifical_lag;
}

