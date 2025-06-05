#ifndef NETMAIN_H
#define NETMAIN_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/e_net_multiplayer_peer.hpp>
#include <godot_cpp/classes/check_button.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/input.hpp>

// #include <godot_cpp/
#include <cstdlib>
#include <map>



#define SERVER_PORT 1337
#define SERVER 1

namespace godot {

struct ClientPlayer {
	RigidBody3D* ptr;
	Vector3 pos;
	Vector3 linear_vel;
	Vector3 angular_vel;
	String name;

	bool is_ready;
	int lap_count;
	ClientPlayer() : ptr(), pos(){}
};

class NetMain : public Node {
	GDCLASS(NetMain, Node)

public:
	NetMain();
	~NetMain();

	void _process(double delta);
	void _ready();

    Ref<MultiplayerAPI> net_api;
	String ip_address = "127.0.0.1"; 
	Dictionary rpc_dict;
	std::map<int, ClientPlayer> roster;
	bool is_client = true;
	bool is_server = false;
	bool player_attached = false;
	bool game_in_session = false;
	int uid;
	int texture_id = 0;
	bool game_started = false;
	bool game_won = false;
	bool should_disable_net = false;

	int artifical_lag = -1;
	float lerp_weight;
	float lerp_threshold;
	
	bool lobby_signal_connected = false;

	int MAX_LAPS = 1;
	void on_client_connect(int client_id);
	void on_client_disconnect(int client_id);
	void on_set_server(bool state);
	void on_set_client(bool state);
	void on_peer_connected(int id);
	void on_peer_disconnected(int id);
	void on_server_info(String new_ip);

	void update_player_info(Vector3 pos, Vector3 linear_vel, Vector3 angular_vel, 
						bool is_ready, int caller_id, int lap_count, String name);
	void on_new_client_info(Vector3 new_pos, Vector3 new_linear_vel, Vector3 new_angular_vel, int new_laps);
	void add_player(int pid, Array current_roster_ids);
	void update_other_rosters(int to_add);

	void on_new_username_created(String new_username);
	void on_new_texture_created(int new_texture);

	void on_player_ready_pressed();
	void start_game();
	void end_game();

	void emit_show_results_signal(const String &results_text);
	void notify_race_finished(int client_id);
	void remove_player(int pid);

	float get_lerp_weight();
	void set_lerp_weight(float new_lerp_weight);
	float get_lerp_threshold();
	void set_lerp_threshold(float new_lerp_threshold);

	int get_artifical_lag();
	void set_artificial_lag(int new_artifical_lag);

	void disable_net(bool shutup);


protected:
	static void _bind_methods();

private:
	String debugString(bool who);

};

}
#endif
