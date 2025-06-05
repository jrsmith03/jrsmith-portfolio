#ifndef MULTIPLAYER_SCREEN_H
#define MULTIPLAYER_SCREEN_H

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
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/control.hpp>

#include <godot_cpp/classes/ip.hpp>


namespace godot {
	class MultiplayerScreen : public Control {
		GDCLASS(MultiplayerScreen, Control);
		
	private:
        Button* startServerButton;
        Button* joinServerButton;
		Button* mainMenuButton;

		TextEdit* serverField;

		void signal_main();

	protected:
		static void _bind_methods();
		
	public:
		void _ready();
		
        void on_main_menu_button_pressed();
        //void on_connect_button_pressed();
		void on_start_server_button_pressed();
		void on_join_server_button_pressed();

	};
}
#endif
