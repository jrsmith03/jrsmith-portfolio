// PUSH_agent.h
#ifndef PUSH_AGENT_H
#define PUSH_AGENT_H

#include "../../AIAgent.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

using namespace godot;

class PushAgent : public AI_Agent {
    GDCLASS(PushAgent, AI_Agent);

protected:
    static void _bind_methods();


public:
    PushAgent();
    ~PushAgent();
    
    void set_agent_spec_file(String a_spec);
    String get_agent_spec_file();

    // State gathering functions
    void gather_state();
    bool is_player_nearby(float distance_threshold);
    void _physics_process(double delta);
    void _ready() override; 

};

#endif // PUSH_AGENT_H
