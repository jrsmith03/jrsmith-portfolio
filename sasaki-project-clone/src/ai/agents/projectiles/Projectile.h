#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp> 
#include <godot_cpp/core/class_db.hpp>
#include "../../../player/Player.h"

using namespace godot;

class Projectile : public RigidBody3D {
    GDCLASS(Projectile, RigidBody3D);

protected:
    static void _bind_methods();

public:
    virtual void _integrate_forces(PhysicsDirectBodyState3D *state) override;

    Projectile();
    ~Projectile();
};

#endif // PROJECTILE_H
