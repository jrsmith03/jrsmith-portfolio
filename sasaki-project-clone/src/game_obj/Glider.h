#ifndef GLIDER_H
#define GLIDER_H

#include <godot_cpp/classes/object.hpp>

#include "godot_cpp/classes/area3d.hpp"

#include <godot_cpp/classes/engine.hpp>

#include "Glider.h"

#include <cstdlib>

namespace godot {

class Glider : public Node3D {
	GDCLASS(Glider, Node3D)

public:
	Glider();
	~Glider();

	void _process(double delta);
	void _ready();
	void on_glider_collect(Area3D* area);

	Area3D* glider_area;

protected:
	static void _bind_methods();

private:

};

}
#endif
