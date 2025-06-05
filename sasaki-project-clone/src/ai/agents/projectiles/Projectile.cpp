#include "Projectile.h"
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


void Projectile::_bind_methods() {
    // No methods to bind for now
}

Projectile::Projectile() {
    set_use_continuous_collision_detection(true);
    set_contact_monitor(true); 
    set_max_contacts_reported(100);
    set_physics_process(true);
}

Projectile::~Projectile() {
}

void Projectile::_integrate_forces(PhysicsDirectBodyState3D *state) {
    int contact_count = state->get_contact_count();

    for (int i = 0; i < contact_count; i++) {
        Object *collider = state->get_contact_collider_object(i);
        if (collider != nullptr && collider->is_class("Player")) {
            
            Vector3 collision_normal = state->get_contact_local_normal(i);
            float bounce_strength = 300.0f;
            RigidBody3D *rigid_body = Object::cast_to<RigidBody3D>(collider);
            if (rigid_body != nullptr) {
                Vector3 impulse_position = state->get_contact_local_position(i);
                // Apply impulse instead of setting velocity
                rigid_body->apply_impulse(impulse_position, collision_normal * bounce_strength);
            }


            // Free the projectile after the collision
            queue_free();
            break; // Exit the loop since we've handled the collision
        }
    }
}
