#include "PushAgent.h"

void PushAgent::_bind_methods() {
}

PushAgent::PushAgent() : AI_Agent() {
    // Define the agent state (this is test code)
    agent_spec_file = "res://agent_trees/push_basic.json";

    decision_tree = memnew(DecisionTree);
    decision_tree->create_decision_tree(agent_spec_file);
    behavior_manager = memnew(BehaviorManager);

}

PushAgent::~PushAgent() {
}

void PushAgent::_physics_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
        gather_state();
        behavior_manager->execute_action(this, delta);
    }
}

void PushAgent::_ready() {
    AI_Agent::_ready(); 
    waypoint_path = "push_path";

}


void PushAgent::gather_state() {
    state["is_player_nearby"] = is_player_nearby(get_pull_distance());
}

bool PushAgent::is_player_nearby(float distance_threshold) {
    TypedArray<Node3D> player_nodes = get_tree()->get_nodes_in_group("players");
    Node3D *player_node = cast_to<Node3D>(player_nodes[0]);
    if (player_node) {
        Vector3 player_position = player_node->get_global_transform().origin;
        Vector3 agent_position = get_global_transform().origin;
        double distance = agent_position.distance_to(player_position);
        return distance < distance_threshold;
    }
    return false;
}
