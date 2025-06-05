#include "Waypoint.h"

using namespace godot;

void Waypoint::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_neighbors", "waypoints"), &Waypoint::set_neighbors);
    ClassDB::bind_method(D_METHOD("get_neighbors"), &Waypoint::get_neighbors);
    ClassDB::bind_method(D_METHOD("add_neighbor", "waypoint"), &Waypoint::add_neighbor);
    ClassDB::bind_method(D_METHOD("reset_neighbors"), &Waypoint::reset_neighbors);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "neighbors", PROPERTY_HINT_ARRAY_TYPE, "Waypoint"), "set_neighbors", "get_neighbors");
}

Waypoint::Waypoint() {
}

Waypoint::~Waypoint() {
}


void Waypoint::set_neighbors(const TypedArray<String>& waypoint) {
    neighbors = waypoint;
}

TypedArray<String> Waypoint::get_neighbors() const {
    return neighbors;
}

void Waypoint::add_neighbor(String waypoint_name) {
    UtilityFunctions::print("Add neighbor: ", waypoint_name, " to waypoint: ", this->get_name());
    if (!neighbors.has(waypoint_name)) {
        neighbors.append(waypoint_name);
    }
    
}

void Waypoint::reset_neighbors() {
    TypedArray<String> empty_array;
    neighbors = empty_array;
}