#ifndef LOBBY_SCENE_H
#define LOBBY_SCENE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/text_edit.hpp>

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/button.hpp>


namespace godot {
	class LobbyScene : public Node3D {
		GDCLASS(LobbyScene, Node3D);
		
	private:


	protected:
		static void _bind_methods();

	public:
		void _ready();
		void _input(const Ref<InputEvent>& event);
	
	};
}
#endif
