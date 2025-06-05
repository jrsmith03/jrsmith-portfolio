#ifndef GRAPH_H
#define GRAPH_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/engine.hpp>



#include <map>
#include <list>
#include "Waypoint.h"

#include <cstdlib>

namespace godot {

class Graph : public Node3D {
	GDCLASS(Graph, Node3D)

public:
	Graph();
	~Graph();

    void add_edge(Waypoint* u, Waypoint* v, double g);

	void print_graph();

    std::map<Waypoint*, std::list<std::tuple<Waypoint*, double, double>>> adjList;

protected:
	static void _bind_methods();

private:
	/* Graph implemented with Adjacency List, where we store a mapping of Waypoints (vertices)
	   to a List containing all of the neighbors

	 		Each vertex tuple: Pointer to the Waypoint (vertex), G (actual distance) 
			                and H (heuristic estimation of cost to goal)
	*/
};

}
#endif
