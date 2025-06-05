#ifndef PAUSE_SCREEN_H
#define PAUSE_SCREEN_H

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
	class PauseScreen : public Control {
		GDCLASS(PauseScreen, Control);

	private:
		Label* pausedLabel;
        Button* resumeButton;
		Button* quitButton;

	protected:
		static void _bind_methods();
	

	public:
		PauseScreen();
		~PauseScreen();
		void _ready();

		void on_resume_button_pressed();
        void on_quit_button_pressed();
		void _input(const Ref<InputEvent>& event);
    
	};

}
#endif
