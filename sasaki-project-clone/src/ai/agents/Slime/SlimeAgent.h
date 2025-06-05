// slime_agent.h
#ifndef SLIME_AGENT_H
#define SLIME_AGENT_H

#include "../../AIAgent.h"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

class SlimeAgent : public AI_Agent {
    GDCLASS(SlimeAgent, AI_Agent);

protected:
    static void _bind_methods();


public:
    SlimeAgent();
    ~SlimeAgent();
    
    void set_agent_spec_file(String a_spec);
    String get_agent_spec_file();
    void _integrate_forces(PhysicsDirectBodyState3D *state);
    RigidBody3D* player;
    bool player_found = false;
    bool is_player_far();

    void _ready();

    // State gathering functions
    void gather_state();
    bool is_player_nearby(float distance_threshold);
    void on_player_grounded(bool b);

    void _physics_process(double delta);


};

#endif // SLIME_AGENT_H
