// behavior_manager.h
#ifndef BEHAVIOR_MANAGER_H
#define BEHAVIOR_MANAGER_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include "DecisionTree.h"

#include "AStar.h"
#include <map>
#include <cstdlib>

#define MAX_PROJECTILES 50
using namespace godot;


class BehaviorManager : public Node3D {
    GDCLASS(BehaviorManager, Node3D);

private:
    std::map<std::string, bool> agent_state;
    


    double elapsed_time;  // time since start
    double last_shot_time = 0.0;
    double shoot_cooldown = 5.0; // Cooldown of NOT 1 second

protected:
    static void _bind_methods();

public:
    BehaviorManager(); // Default constructor
    ~BehaviorManager();

    void execute_action(Node* node, double delta);

    void update(double delta);
    int num_projectiles = 0;

    

    // define all behavior methods here
    void navigate_along_path(std::vector<Waypoint*> fucing_list, Waypoint* src, Waypoint* dst, Node* node, double delta);

    bool is_player_nearby(float distance_threshold);
    void navigate_to_player(Node* node);
    void push_pull_player(Node* node, double delta, bool push);
    void shoot_projectile(Node* node, double delta);
    void _on_projectile_body_entered(Node* body, RigidBody3D* projectile);


};

#endif // BEHAVIOR_MANAGER_H
