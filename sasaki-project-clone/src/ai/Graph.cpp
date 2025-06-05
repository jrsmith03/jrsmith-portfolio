#include "Graph.h"

using namespace godot;

// Adapted from https://www.geeksforgeeks.org/implementation-of-graph-in-cpp/
void Graph::_bind_methods() {

}

Graph::Graph() {
}

Graph::~Graph() {
}

void Graph::add_edge(Waypoint* u, Waypoint* v, double g) {
    if (u == nullptr || v == nullptr) {
        return;
    }
    std::tuple<Waypoint*, double, double> vec_v{u, g, 0.0f};
    std::tuple<Waypoint*, double, double> vec_u{v, g, 0.0f};
    
    // Add edge from u to v
    adjList[u].push_back(vec_u);
    // // Add edge from v to u because the graph is undirected
    adjList[v].push_back(vec_v);
}

// Function to print the adjacency list representation
// of the graph
void Graph::print_graph()
{
    // Iterate over each vertex
    for (auto i : adjList) {
        // Print the vertex
        UtilityFunctions::print("NODE ", i.first->get_name(), " -> ");
        // Iterate over the connected vertices
        for (auto j : i.second) {
            // Print the connected vertex
            UtilityFunctions::print("   ", std::get<0>(j)->get_name());
            UtilityFunctions::print("COST: ", std::get<1>(j));

        }
    }
}