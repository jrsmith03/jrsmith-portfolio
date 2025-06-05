#include "Glider.h"

using namespace godot;

void Glider::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_glider_collect", "area"), &Glider::on_glider_collect);

    ADD_SIGNAL(MethodInfo("glider_equip"));
}

Glider::Glider() {
}

Glider::~Glider() {
}

void Glider::_ready() {
    glider_area = get_node<Area3D>("./Area3D");
    if (glider_area) {
        glider_area->connect("area_entered", Callable(this, "on_glider_collect"));
    } 
}
void Glider::_process(double delta) {
    float change = 0.1;
    if (this->get_rotation().y >= 360) {
        change = -change;
    }
    this->set_rotation(get_rotation() + Vector3(0, change, 0));
}


void Glider::on_glider_collect(Area3D* area) {
    emit_signal("glider_equip");
    this->set_visible(false);
}
