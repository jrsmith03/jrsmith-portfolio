// behavior_manager.cpp
#include "BehaviorManager.h"
#include "AIAgent.h"
#include "agents/projectiles/Projectile.h"
/*
    This is the Central Executive for all agents.
    The specifications of all agent methods are found in this file.
    Each agent will only execute a subset of the actions, as specified by their Decision Trees.
*/

void BehaviorManager::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("update", "delta"), &BehaviorManager::update);
}

// Default constructor
BehaviorManager::BehaviorManager() {
    
}


BehaviorManager::~BehaviorManager() {
    
}


void BehaviorManager::execute_action(Node* node, double delta) {
    // We have to walk down the chain of inheritance because we can't include AIAgent in this file (or else we get a circular import!)
    AI_Agent* agent = static_cast<AI_Agent*>(node);
    if (agent->decision_tree == nullptr) return; 
    AgentAction::actions behavior = agent->decision_tree->make_decision(agent->decision_tree->root, agent->state);
    switch (behavior) {
        case AgentAction::NONE:
            break;
        case AgentAction::PATHFIND:
        {
            std::vector<Waypoint*> cur_wps;
            if (agent->reached_momentary_goal) {
                // Pick a new goal!
                
                for(int i = 0; i < 2; i++){
                    // Waypoint* waypoint = cast_to<Waypoint>(waypoints[i]);
                    UtilityFunctions::print("momentar a wp");
                    if(agent->my_waypoints[i]) {
                        cur_wps.push_back(agent->my_waypoints[i]);
                        UtilityFunctions::print("fuckingholygoddamnshit");

                    } 
                }
                agent->reached_momentary_goal = false;
            }
            agent->current_path = agent->nav->path_to_goal(cur_wps[0], cur_wps.back());
            navigate_along_path(agent->current_path, agent->current_path[0], agent->current_path.back(), node, delta);

            break;
        }
        case AgentAction::PURSUIT:
            navigate_to_player(node);
            break;
        case AgentAction::PULL: 
            push_pull_player(node, delta, false);
            break;
        case AgentAction::WANDER: 
        {
            Waypoint* me = static_cast<Waypoint*>(node);

            if (agent->reached_momentary_goal) {
                // Pick a new goal!
                
                for(int i = 0; i < 2; i++){                    
                    if(agent->my_waypoints[i]) {
                        agent->current_path.push_back(agent->my_waypoints[i]);
                    } 
                }
                agent->reached_momentary_goal = false;
            }

            navigate_along_path(agent->current_path, me, agent->current_path.back(), node, delta);

            break;
        }
        case AgentAction::PUSH: 
            push_pull_player(node, delta, true);
            break;
        case AgentAction::SHOOT: 
            shoot_projectile(node, delta);

            break;
        default:
            break;
    }
}

void BehaviorManager::update(double delta) {
    // Accumulate elapsed time
    elapsed_time += delta;
}

// Add behaviors here!
void BehaviorManager::navigate_along_path(std::vector<Waypoint*> test_waypoints, Waypoint* src, Waypoint* dst, Node* node, double delta) {
    AI_Agent* agent = static_cast<AI_Agent*>(node);

    if (agent->pick_intermediate_dest) {
        if (agent->current_path.size() > 0) {
            agent->intermediate_dest = agent->current_path.back(); 
            agent->current_path.pop_back();
        } else {
            agent->reached_momentary_goal = true;
        }

    }
    agent->pick_intermediate_dest = agent->nav->nav_to_goal(agent, agent->intermediate_dest, delta);

}

void BehaviorManager::navigate_to_player(Node* node) {
    AI_Agent* agent = static_cast<AI_Agent*>(node);
    if (agent == nullptr) {
        UtilityFunctions::printerr("Agent is null, aborting navigation.");
        return;
    }

    TypedArray<Node3D> player_nodes = node->get_tree()->get_nodes_in_group("players");
    Node3D *player_node = cast_to<Node3D>(player_nodes[0]);
    if (player_node == nullptr) {
        UtilityFunctions::printerr("Player node not found, aborting navigation.");
        return;
    }

    Vector3 player_position = player_node->get_global_transform().origin;
    Vector3 agent_position = agent->get_global_transform().origin;

    Vector3 direction = (player_position - agent_position).normalized();
    agent->set_linear_velocity(direction * 75.0f); // Move towards player at a fixed speed
}


void BehaviorManager::push_pull_player(Node *node, double delta, bool push) {
    AI_Agent *agent = Object::cast_to<AI_Agent>(node);
    if (agent == nullptr) {
        UtilityFunctions::printerr("Agent is null, aborting pull behavior.");
        return;
    }

    TypedArray<Node3D> player_nodes = node->get_tree()->get_nodes_in_group("players");
    Node3D *player_node = cast_to<Node3D>(player_nodes[0]);
    if (player_node == nullptr) {
        UtilityFunctions::printerr("Player node not found, aborting pull behavior.");
        return;
    }

    Vector3 agent_position = agent->get_global_transform().get_origin();
    Vector3 player_position = player_node->get_global_transform().get_origin();

    // Calculate direction from player to NPC
    Vector3 direction;
    if (push){
        // if push is set to true
        direction = (player_position - agent_position).normalized();
    } else {
        // if push is set to false
        // means pulling
        direction = (agent_position - player_position).normalized();
    }
    

    // Define the pull strength (adjustable as needed)
    float pull_strength = 10.0f;

    // Apply pull by changing the player's position incrementally
    Vector3 new_player_position = player_position + direction * pull_strength * delta;
    Transform3D new_transform = player_node->get_global_transform();
    new_transform.origin = new_player_position;
    player_node->set_global_transform(new_transform);
}

void BehaviorManager::shoot_projectile(Node* node, double delta) {

    AI_Agent* agent = Object::cast_to<AI_Agent>(node);
    if (agent == nullptr) {
        UtilityFunctions::printerr("Agent is null, aborting shoot behavior.");
        return;
    }

    float projectile_size = agent->get_projectile_size();
    float projectile_speed = agent->get_projectile_speed();
    float projectile_duration = agent->get_projectile_duration();
    // Check if enough time has passed since the last shot
    if (elapsed_time - last_shot_time < shoot_cooldown) {
        return; // Cooldown not met, do not shoot
    }

    // Update the last shot time
    last_shot_time = elapsed_time;

    // Locate the player node
    TypedArray<Node3D> player_nodes = node->get_tree()->get_nodes_in_group("players");
    Node3D *player_node = cast_to<Node3D>(player_nodes[0]);
    if (player_node == nullptr) {
        UtilityFunctions::printerr("Player node not found, aborting shoot behavior.");
        return;
    }

    // Create a new Projectile
    Projectile* projectile = memnew(Projectile());
    projectile->set_name("Projectile");

    // Set gravity scale to 0 (no gravity)
    projectile->set_gravity_scale(0.0f);

    // Set collision layer and mask
    // Adjust these values based on your project's settings
    projectile->set_collision_layer(2);
    projectile->set_collision_mask(2);

    // Add the projectile to the scene
    agent->add_child(projectile);

    // Set initial position of the projectile
    Transform3D start_transform = agent->get_global_transform();
    projectile->set_global_transform(start_transform);

    // Add a CollisionShape3D to the projectile
    CollisionShape3D* collision_shape = memnew(CollisionShape3D());
    

    // Create a sphere shape and assign it to the collision shape
    Ref<SphereShape3D> sphere_shape;
    sphere_shape.instantiate();
    sphere_shape->set_radius(projectile_size);
    collision_shape->set_shape(sphere_shape);
    projectile->add_child(collision_shape);

    // Add a MeshInstance3D to the projectile for visibility
    MeshInstance3D* mesh_instance = memnew(MeshInstance3D());
    Ref<SphereMesh> sphere_mesh;
    sphere_mesh.instantiate();
    sphere_mesh->set_radius(projectile_size);
    sphere_mesh->set_height(projectile_size*2);
    mesh_instance->set_mesh(sphere_mesh);
    projectile->add_child(mesh_instance);

    // Calculate direction towards the player
    Vector3 player_position = player_node->get_global_transform().origin;
    Vector3 agent_position = agent->get_global_transform().origin;
    Vector3 direction = (player_position - agent_position).normalized();

    // Apply linear velocity to the projectile
    projectile->set_linear_velocity(direction * projectile_speed);

    // Set a timer to destroy the projectile after 5 seconds
    Timer* timer = memnew(Timer());
    timer->set_wait_time(projectile_duration);
    timer->set_one_shot(true);
    projectile->add_child(timer);
    timer->connect("timeout", Callable(projectile, "queue_free"));
    timer->start();
}


