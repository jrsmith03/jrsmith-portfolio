#ifndef END_SCREEN_H
#define END_SCREEN_H

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
	class EndScreen : public Control {
		GDCLASS(EndScreen, Control);

	private:
		Label* gameOverLabel;
        Button* restartButton;
		Button* quitButton;

	protected:
		static void _bind_methods();
	

	public:
		EndScreen();
		~EndScreen();
		void _ready();

		void on_restart_button_pressed();
        void on_quit_button_pressed();
		void _input(const Ref<InputEvent>& event);
    
	};

}
#endif
