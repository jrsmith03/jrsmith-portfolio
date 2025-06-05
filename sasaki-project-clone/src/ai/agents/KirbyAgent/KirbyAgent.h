// KirbyAgent.h
#ifndef KIRBY_AGENT_H
#define KIRBY_AGENT_H

#include "../../AIAgent.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <godot_cpp/classes/standard_material3d.hpp>

using namespace godot;

class KirbyAgent : public AI_Agent {
    GDCLASS(KirbyAgent, AI_Agent);

protected:
    static void _bind_methods();


public:
    KirbyAgent();
    ~KirbyAgent();
    
    void set_agent_spec_file(String a_spec);
    String get_agent_spec_file();

    bool player_found = false;
    RigidBody3D* player;
    // State gathering functions
    void gather_state();
    bool is_player_nearby(float distance_threshold);
    void _physics_process(double delta);
    void _ready() override;


};

#endif // KIRBY_AGENT_H
