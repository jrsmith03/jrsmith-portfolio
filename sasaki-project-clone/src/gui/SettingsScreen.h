#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

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
#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/option_button.hpp>

namespace godot {
	class SettingsScreen : public Control {
		GDCLASS(SettingsScreen, Control);

	private:
        Button* mainMenuButton;

	protected:
		static void _bind_methods();

	public:
		void _ready();
		
        void on_main_menu_button_pressed();
	};
}
#endif
