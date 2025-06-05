#include "register_types.h"

using namespace godot;

void initialize_gui_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(GUI);
	GDREGISTER_CLASS(PauseScreen);
	GDREGISTER_CLASS(EndScreen);
	GDREGISTER_CLASS(StartScreen);
	GDREGISTER_CLASS(SettingsScreen);
	GDREGISTER_CLASS(MultiplayerScreen);
	GDREGISTER_CLASS(LobbyScreen);
	GDREGISTER_CLASS(MainMenu);
	GDREGISTER_CLASS(LobbyScene);
	GDREGISTER_CLASS(ResultsScreen);
}

void uninitialize_gui_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT gui_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_gui_module);
	init_obj.register_terminator(uninitialize_gui_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
