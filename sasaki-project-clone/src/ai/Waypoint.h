#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/engine.hpp>

#include <cstdlib>

namespace godot {

class Waypoint : public Node3D {
	GDCLASS(Waypoint, Node3D)

	public: 
		Waypoint();
		~Waypoint();

		void set_neighbors(const TypedArray<String>& waypoint);
		TypedArray<String> get_neighbors() const;
		void add_neighbor(String waypoint_name);
		void reset_neighbors();


	protected:
		static void _bind_methods();

	private:
		TypedArray<String> neighbors;

};

}
#endif
