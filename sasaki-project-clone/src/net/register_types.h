#ifndef REGISTER_TYPES_H
#define REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void initialize_net_module(ModuleInitializationLevel p_level);
void uninitialize_net_module(ModuleInitializationLevel p_level);

#endif // REGISTER_TYPES_H