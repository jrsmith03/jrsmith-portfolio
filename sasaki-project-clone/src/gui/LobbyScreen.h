#ifndef LOBBY_SCREEN_H
#define LOBBY_SCREEN_H

#include <godot_cpp/classes/node.hpp>
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
	class LobbyScreen : public Control {
		GDCLASS(LobbyScreen, Control);

	private:
        Button* exitButton;
        Button* startButton;

	protected:
		static void _bind_methods();

	public:
		void _ready();
		
        void on_exit_button_pressed();
        void on_start_button_pressed();
	};
}
#endif
