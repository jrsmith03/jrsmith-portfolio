#ifndef AI_AGENT_H
#define AI_AGENT_H

#include <godot_cpp/core/class_db.hpp>
#include "TreeNode.h"
#include "DecisionTree.h"
#include "BehaviorManager.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include "Waypoint.h"

using namespace godot;

class AI_Agent : public RigidBody3D {
    GDCLASS(AI_Agent, RigidBody3D);

protected:
    static void _bind_methods();

    AI_Agent();
    ~AI_Agent();
private:

public:
    String agent_spec_file;
    // Godot's lifecycle methods
    void _ready() override;
    void _process(double delta) override;

    BehaviorManager* behavior_manager;
    DecisionTree* decision_tree;
    AStar* nav;
    bool reached_momentary_goal = true;
    bool pick_intermediate_dest = true;
    Waypoint* intermediate_dest;

    std::vector<Waypoint*> my_waypoints;

    std::vector<Waypoint*> current_path;
    std::map<std::string, bool> state;
    String waypoint_path;
    
    // behavior specific variables and functions



    // Pull-Push behavior functions
    void set_pull_distance(float distance);
    float get_pull_distance() const;
    void update_pull_circle();

    float get_projectile_size() const { return projectile_size; }
    void set_projectile_size(float size) { projectile_size = size; }

    float get_projectile_duration() const { return projectile_duration; }
    void set_projectile_duration(float duration) { projectile_duration = duration; }

    float get_projectile_speed() const { return projectile_speed; }
    void set_projectile_speed(float speed) { projectile_speed = speed; }
    MeshInstance3D* pull_circle;

private: // exposed variables
    float projectile_size;
    float projectile_duration;
    float projectile_speed;
    float pull_distance;
    
    
    
};

#endif // AI_AGENT_H
