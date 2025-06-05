#ifndef GDEXAMPLE_REGISTER_TYPES_H
#define GDEXAMPLE_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

#include "GUI.h"
#include "StartScreen.h"
#include "SettingsScreen.h"
#include "PauseScreen.h"
#include "EndScreen.h"
#include "MultiplayerScreen.h"
#include "LobbyScreen.h"
#include "MainMenu.h"
#include "LobbyScene.h"
#include "ResultsScreen.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

using namespace godot;

void initialize_gui_module(ModuleInitializationLevel p_level);
void uninitialize_gui_module(ModuleInitializationLevel p_level);

#endif // GDEXAMPLE_REGISTER_TYPES_H
