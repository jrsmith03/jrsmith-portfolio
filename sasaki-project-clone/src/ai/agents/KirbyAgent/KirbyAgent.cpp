// KirbyAgent.cpp
#include "KirbyAgent.h"


void KirbyAgent::_bind_methods() {
}

KirbyAgent::KirbyAgent() : AI_Agent() {
    agent_spec_file = "res://agent_trees/kirby_basic.json";

    decision_tree = memnew(DecisionTree);
    decision_tree->create_decision_tree(agent_spec_file);
    behavior_manager = memnew(BehaviorManager);
    waypoint_path = "kirby_path";

}

KirbyAgent::~KirbyAgent() {
}

void KirbyAgent::_physics_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
        gather_state();
        behavior_manager->execute_action(this, delta);
    }
}

void KirbyAgent::_ready() {
    // Ref<StandardMaterial3D> material = memnew(StandardMaterial3D);
    // material->set_albedo(Color(1.0, 0.5, 0, 0.2)); // make circle red
    // material->set_transparency(StandardMaterial3D::TRANSPARENCY_ALPHA);
    // material->set_cull_mode(StandardMaterial3D::CULL_DISABLED);
    // pull_circle->set_material_override(material);
    AI_Agent::_ready();

}

void KirbyAgent::gather_state() {
    state["is_player_nearby"] = is_player_nearby(50);
}


bool KirbyAgent::is_player_nearby(float distance_threshold) {
     if (!player_found) {
        Node* player_node = get_node<Node>("/root/World/Player"); 
        if (player_node) {
            player_found = true;
            player = static_cast<RigidBody3D*>(player_node);
        }
    } else {
        Vector3 player_position = player->get_global_transform().origin;
        Vector3 agent_position = get_global_transform().origin;
        double distance = agent_position.distance_to(player_position);
        return distance < distance_threshold;
    }
    return false;   
}
