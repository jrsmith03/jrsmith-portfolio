#ifndef ASTAR_H
#define ASTAR_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "Waypoint.h"

#include "Graph.h"
#include <queue>

#include <cstdlib>

using namespace godot;

class AStar : public Node3D {
GDCLASS (AStar, Node3D)

	public: 
		AStar();
		~AStar();
		Graph* search_space;

		void _ready();
		std::vector<Waypoint*> path_to_goal(Waypoint* start, Waypoint* goal);
		bool nav_to_goal(Node3D* victim, Waypoint* dest, double delta);

		double euclidean(Vector3 a, Vector3 b);
		double manhattan(Vector3 a, Vector3 b);

	protected:
		static void _bind_methods();

};


#endif
