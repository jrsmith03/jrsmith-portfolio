// Shoot_agent.h
#ifndef Shoot_AGENT_H
#define Shoot_AGENT_H

#include "../../AIAgent.h"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

class ShootAgent : public AI_Agent {
    GDCLASS(ShootAgent, AI_Agent);

protected:
    static void _bind_methods();


public:
    ShootAgent();
    ~ShootAgent();
    
    void set_agent_spec_file(String a_spec);
    String get_agent_spec_file();
    RigidBody3D* player;
    bool player_found = false;
    void _ready();
    bool is_player_far();


    // State gathering functions
    void gather_state();
    bool is_player_nearby(float distance_threshold);
    void _physics_process(double delta);
    

};

#endif // Shoot_AGENT_H
