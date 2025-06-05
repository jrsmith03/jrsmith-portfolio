#ifndef World_H
#define World_H

#include "godot_cpp/classes/node3d.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/control.hpp>

namespace godot {

class World : public Node3D {
	GDCLASS(World, Node3D)

public:
	World();
	~World();

	void _ready() override;
	void _process(double delta) override;
	void set_singleplayer_level();
	void set_multiplayer_level();
	void set_multiplayer_screen();
	void set_lobby_level();
    Control* menu; 
	Control* resultsScreen;
	bool is_singleplayer = false;
	int set_level(String level_path);
	void set_environment();



protected:
	static void _bind_methods();

private:

};

}
#endif
