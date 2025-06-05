#include "AIAgent.h"

void AI_Agent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_projectile_size"), &AI_Agent::get_projectile_size);
    ClassDB::bind_method(D_METHOD("set_projectile_size", "size"), &AI_Agent::set_projectile_size);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_size"), "set_projectile_size", "get_projectile_size");

    ClassDB::bind_method(D_METHOD("get_projectile_duration"), &AI_Agent::get_projectile_duration);
    ClassDB::bind_method(D_METHOD("set_projectile_duration", "duration"), &AI_Agent::set_projectile_duration);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_duration"), "set_projectile_duration", "get_projectile_duration");

    ClassDB::bind_method(D_METHOD("set_pull_distance", "distance"), &AI_Agent::set_pull_distance);
    ClassDB::bind_method(D_METHOD("get_pull_distance"), &AI_Agent::get_pull_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pull_distance"), "set_pull_distance", "get_pull_distance");


    ClassDB::bind_method(D_METHOD("get_projectile_speed"), &AI_Agent::get_projectile_speed);
    ClassDB::bind_method(D_METHOD("set_projectile_speed", "speed"), &AI_Agent::set_projectile_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_speed"), "set_projectile_speed", "get_projectile_speed");
}

AI_Agent::AI_Agent() : behavior_manager(nullptr), pull_distance(50.0f), pull_circle(nullptr), projectile_size(5.0), projectile_duration(5.0), projectile_speed(20.0) {
    // Initialize pull_circle for visualizing pull distance
    // pull_circle = memnew(MeshInstance3D);
    // add_child(pull_circle);

    // // Set up a semi-transparent material
    // Ref<StandardMaterial3D> material = memnew(StandardMaterial3D);
    // material->set_albedo(Color(0.0, 0.5, 1.0, 0.2)); // Light blue, semi-transparent
    // material->set_transparency(StandardMaterial3D::TRANSPARENCY_ALPHA);
    // material->set_cull_mode(StandardMaterial3D::CULL_DISABLED);

    // // Set up the sphere mesh
    // // Ref<SphereMesh> sphere_mesh = memnew(SphereMesh);
    // sphere_mesh->set_radius(pull_distance);
    // sphere_mesh->set_height(pull_distance);
    // pull_circle->set_mesh(sphere_mesh);
    // pull_circle->set_material_override(material);
    // pull_circle->set_visible(true);


}

AI_Agent::~AI_Agent() {
    // Safely delete the behavior manager if it exists
}

void AI_Agent::_ready() {
    set_collision_layer(1);
    set_collision_mask(1);
    update_pull_circle();
    nav = static_cast<AStar*>(get_node<Node3D>("AStar"));
    
}

void AI_Agent::_process(double delta) {
    behavior_manager->update(delta);
}

void AI_Agent::update_pull_circle() {
    if (pull_circle) {
        
        Ref<SphereMesh> sphere_mesh = memnew(SphereMesh);
        sphere_mesh->set_radius(pull_distance);
        sphere_mesh->set_height(pull_distance*2);
        pull_circle->set_mesh(sphere_mesh);
    }
}

void AI_Agent::set_pull_distance(float distance) {
    pull_distance = distance;
    update_pull_circle();
}

float AI_Agent::get_pull_distance() const {
    return pull_distance;
}
