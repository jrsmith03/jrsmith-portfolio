#ifndef GUI_H
#define GUI_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>


#include <godot_cpp/classes/text_edit.hpp>

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

namespace godot {
	class GUI : public Control {
		GDCLASS(GUI, Control);

	private:
		Control* score_display;
		Engine* state;
		Control* pause_menu;
		Control* end_menu;
		Node3D* level_ref;

		Control* glider_time_remaining;
		Control* checkpoint_time_remaining;

		TextureRect* speedometer_hand;

	protected:
		static void _bind_methods();
	

	public:
		GUI();
		~GUI();
		void _ready();
		void _process(double delta);

		bool fker_found = false;
		void on_score_updated(int new_score);
		void on_speed_updated(float new_speed);
		void update_glide_timer_remaining(float time_left);
		void on_player_death();
		void on_glider_equip();
		void _input(const Ref<InputEvent>& event);
		void update_checkpoint_time(float time_left);
		float remap_range(float val, float srcLo, float srcHi, float destLo, float destHi);
	};

}
#endif
