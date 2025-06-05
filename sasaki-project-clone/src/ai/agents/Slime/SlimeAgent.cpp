// slime_agent.cpp
#include "SlimeAgent.h"


void SlimeAgent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_player_grounded", "b"),
        &SlimeAgent::on_player_grounded);
}

SlimeAgent::SlimeAgent() : AI_Agent() {
    agent_spec_file = "res://agent_trees/slime.json";

    decision_tree = memnew(DecisionTree);
    decision_tree->create_decision_tree(agent_spec_file);
    behavior_manager = memnew(BehaviorManager);
    set_use_continuous_collision_detection(true);
    set_contact_monitor(true); 
    set_max_contacts_reported(100);
    set_physics_process(true);

    waypoint_path = "level0_waypoints";
}

SlimeAgent::~SlimeAgent() {
}

void SlimeAgent::_ready() {
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

void SlimeAgent::_physics_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
        gather_state();
        behavior_manager->execute_action(this, delta);
    }
}

void SlimeAgent::gather_state() {
    state["is_player_far"] = !is_player_nearby(500);                    // > 300 units away
    state["is_player_nearby"] = is_player_nearby(500) && !is_player_nearby(150);  // between 50-300 units
    state["is_player_close"] = is_player_nearby(150);                    // < 50 units
}


bool SlimeAgent::is_player_nearby(float distance_threshold) {
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


bool SlimeAgent::is_player_far() {
    return !is_player_nearby(250);
}

void SlimeAgent::on_player_grounded(bool b) {
    state["is_player_grounded"] = b;
}

void SlimeAgent::_integrate_forces(PhysicsDirectBodyState3D *state) {
    int contact_count = state->get_contact_count();

    for (int i = 0; i < contact_count; i++) {
        Object *collider = state->get_contact_collider_object(i);

        if (collider != nullptr && collider->is_class("Player")) {
            
            Vector3 collision_normal = state->get_contact_local_normal(i);
            float bounce_strength = 500.0f;
            RigidBody3D *rigid_body = Object::cast_to<RigidBody3D>(collider);
            if (rigid_body != nullptr) {
                Vector3 impulse_position = state->get_contact_local_position(i);

                // Apply impulse instead of setting velocity
                rigid_body->apply_impulse(impulse_position, collision_normal * bounce_strength);
            }

            break; // Exit the loop since we've handled the collision
        }
    }
}