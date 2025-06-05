#ifndef START_SCREEN_H
#define START_SCREEN_H

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
#include <godot_cpp/classes/parallax_background.hpp>


namespace godot {
	class StartScreen : public Control {
		GDCLASS(StartScreen, Control);
		
	private:
        Button* soloButton;
        Button* multiplayerButton;
        Button* settingsButton;
		Button* quitButton;

	protected:
		static void _bind_methods();

	public:
		void _ready();
		
		void on_solo_button_pressed();
		void on_multiplayer_button_pressed();
		void on_settings_button_pressed();
        void on_quit_button_pressed();
		void on_server_button_pressed();
		void on_client_click();

	};
}
#endif
