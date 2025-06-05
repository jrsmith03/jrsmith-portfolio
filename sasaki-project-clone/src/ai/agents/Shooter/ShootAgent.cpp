// Shoot_agent.cpp
#include "ShootAgent.h"


void ShootAgent::_bind_methods() {
}

ShootAgent::ShootAgent() : AI_Agent(){
    agent_spec_file = "res://agent_trees/shoot_basic.json";
    decision_tree = memnew(DecisionTree);
    behavior_manager = memnew(BehaviorManager);

    decision_tree->create_decision_tree(agent_spec_file);
    set_pull_distance(0);
    waypoint_path = "shoot_path";

}

ShootAgent::~ShootAgent() {
}

void ShootAgent::_physics_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
        gather_state();
        behavior_manager->execute_action(this, delta);
    }
}

void ShootAgent::gather_state() {
    state["is_player_far"] = !is_player_nearby(500);                    // > 300 units away
    state["is_player_nearby"] = is_player_nearby(500) && !is_player_nearby(150);  // between 50-300 units
    state["is_player_close"] = is_player_nearby(150);                    // < 50 units

}
void ShootAgent::_ready() {
    for(int i = 0; i < 2; i++){
        int x = 0;
        Vector3 coord = get_global_position();
        if (i == 0) x = 50;
        if (i == 1) x = -50;
        Ref<PackedScene> nav1 = ResourceLoader::get_singleton()->load("res://gd-level/Waypoint.tscn");
        Node* n1 = nav1->instantiate();
        Waypoint* n1u = static_cast<Waypoint*>(n1);
        n1u->set_global_position(Vector3(coord.x + x, coord.y+1, coord.z));

        my_waypoints.push_back(n1u);
        UtilityFunctions::print("add a wp");
        // Waypoint* waypoint = cast_to<Waypoint>(waypoints[i]);
        // if(waypoint) {
        // } 
    }
}
bool ShootAgent::is_player_far() {
    return !is_player_nearby(250);
}
bool ShootAgent::is_player_nearby(float distance_threshold) {
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



