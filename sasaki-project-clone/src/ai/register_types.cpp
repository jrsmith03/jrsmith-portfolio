// register_types.cpp
#include "register_types.h"

#include "AIAgent.h"
#include "agents/Slime/SlimeAgent.h"
#include "agents/KirbyAgent/KirbyAgent.h"
#include "agents/Push/PushAgent.h"
#include "agents/Shooter/ShootAgent.h"

#include "agents/projectiles/Projectile.h"

#include "DecisionTree.h"
#include "BehaviorManager.h"
#include "Waypoint.h"
#include "AStar.h"
#include "Graph.h"


using namespace godot;

void initialize_ai_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    // Internal structures
	GDREGISTER_CLASS(TreeNode);
	GDREGISTER_CLASS(AgentAction);
    GDREGISTER_CLASS(DecisionTree);   
    GDREGISTER_CLASS(BehaviorManager);
    
    // Agent classes
    GDREGISTER_ABSTRACT_CLASS(AI_Agent);
    GDREGISTER_CLASS(SlimeAgent);
    GDREGISTER_CLASS(KirbyAgent);
    GDREGISTER_CLASS(PushAgent);

    GDREGISTER_CLASS(ShootAgent);
    GDREGISTER_CLASS(Projectile);
    GDREGISTER_CLASS(Waypoint);
    GDREGISTER_CLASS(AStar);
    GDREGISTER_CLASS(Graph);
}

void uninitialize_ai_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT ai_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                            const GDExtensionClassLibraryPtr p_library,
                                            GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_ai_module);
        init_obj.register_terminator(uninitialize_ai_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
