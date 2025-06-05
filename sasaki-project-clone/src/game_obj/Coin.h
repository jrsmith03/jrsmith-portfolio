#ifndef COIN_H
#define COIN_H

#include <godot_cpp/classes/object.hpp>
#include "godot_cpp/classes/area3d.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../player/Player.h"

#include "Coin.h"

#include <cstdlib>


#define COIN_VALUE 1

namespace godot {

class Coin : public Node3D {
	GDCLASS(Coin, Node3D)

public:
	Coin();
	~Coin();

	void _process(double delta);
	void _ready();
	void on_coin_collect(Node3D* body);

	Area3D* coin_area;
	Engine* state;

	int value;

protected:
	static void _bind_methods();

private:

};

}
#endif
