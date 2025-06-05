#include "register_types.h"

#include "Platform.h"
#include "Level.h"
#include "World.h"
#include "ProceduralPlane.h"
#include "CheckpointMarker.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>
using namespace godot;

void initialize_platform_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(Platform);
	GDREGISTER_CLASS(Level);
	GDREGISTER_CLASS(World);
	GDREGISTER_CLASS(ProceduralPlane);
	GDREGISTER_CLASS(CheckpointMarker);
	}

void uninitialize_platform_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
	// Initialization.
	GDExtensionBool GDE_EXPORT platform_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_platform_module);
		init_obj.register_terminator(uninitialize_platform_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
