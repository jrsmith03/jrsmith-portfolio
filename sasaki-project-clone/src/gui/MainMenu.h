#ifndef MAIN_MENU_H
#define MAIN_MENU_H

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

#include "GUI.h"
#include "StartScreen.h"
#include "SettingsScreen.h"
#include "PauseScreen.h"
#include "EndScreen.h"
#include "MultiplayerScreen.h"
#include "LobbyScreen.h"

namespace godot {
	class MainMenu : public Control {
		GDCLASS(MainMenu, Control);
		
	private:
		String server_ip;
		String username;
		uint8_t selected_texture;
		uint8_t selected_protocol;
		uint8_t selected_level;
		void close_menus();

	protected:
		static void _bind_methods();
		
	public:
		void _ready();

		void on_server_update(String new_server);
		void on_protocol_update(uint8_t new_protocol);
		void on_username_update(String new_username);
		void on_texture_update(uint8_t new_texture);
		void on_level_update(uint8_t new_level);
		void on_load_start();
		void on_load_settings();
		void on_load_multiplayer();
		void on_load_lobby();
	};
}
#endif
