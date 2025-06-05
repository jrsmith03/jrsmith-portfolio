#include "AStar.h"
#include <algorithm>
using namespace godot;

struct PWaypoint {
    Waypoint* waypoint;
    double priority;

    PWaypoint(Waypoint* wp, double p) : waypoint(wp), priority(p) {}
    
    bool operator==(const PWaypoint& other) const {
        return waypoint == other.waypoint && priority == other.priority;
    }
};

struct wpcomp {
    bool operator()(const PWaypoint& w1, const PWaypoint& w2) {
        return w1.priority > w2.priority;
    }
};
void AStar::_bind_methods() {

}

AStar::AStar() {
}

AStar::~AStar() {
}

void AStar::_ready() {
    search_space = memnew(Graph);
    std::map<String, Waypoint*> wp_map;
    TypedArray<Waypoint> waypoints = get_tree()->get_nodes_in_group("nav");
    for(int i = 0; i < waypoints.size(); i++){
        Waypoint* waypoint = cast_to<Waypoint>(waypoints[i]);
        wp_map[waypoint->get_name()] = waypoint;
    }

    
    for (const auto& pair  : wp_map) {
        TypedArray<String> neighbors = pair.second->get_neighbors();
        for (int i = 0; i < neighbors.size(); i++) {
            String neighbor = static_cast<String>(neighbors[i]);
            search_space->add_edge(pair.second, wp_map[neighbor], euclidean(pair.second->get_global_position(), wp_map[neighbor]->get_global_position()));
        }
        
    }
    search_space->print_graph();
}


std::vector<Waypoint*> AStar::path_to_goal(Waypoint* start, Waypoint* goal){
    std::unordered_map<Waypoint*, Waypoint*> came_from;    // Notion of path ordering
    std::priority_queue<PWaypoint, std::vector<PWaypoint>, wpcomp> open;
    std::list<PWaypoint> unordered_open;    // Sister structure for traversal
    std::list<Waypoint*> closed;

    PWaypoint p_start = PWaypoint(start, 0);
    open.push(p_start);
    unordered_open.push_back(p_start);

    PWaypoint pcur(nullptr, 0);
    Waypoint* cur;
    while (open.size() > 0) {
        pcur = open.top();
        cur = pcur.waypoint;
        open.pop();
        unordered_open.remove(pcur);

        closed.push_back(cur);
        if (cur == goal) {
            // Reconstruct the path that got to cur.
            std::vector<Waypoint*> shortest_path;
            while (cur != nullptr) {
                shortest_path.push_back(cur);
                cur = came_from[cur];
            }
            return shortest_path;
        }
        // Vertex = (Waypoint, G, H)
        for (auto neighbor : search_space->adjList[cur]) {
            Waypoint* neighbor_pointer = std::get<0>(neighbor);
            bool neighbor_not_closed = true;
            for (auto closed_elem : closed) {
                if (closed_elem == neighbor_pointer) {
                    neighbor_not_closed = false;
                }
            }
            if (!neighbor_not_closed) {
                continue;
            }

            // Current cost for neighbor + cost between neighbor and current node
            double new_cost = std::get<1>(neighbor) + pcur.priority;   
            // Cost from the neighbor to the end
            double heuristic_cost = euclidean(neighbor_pointer->get_global_position(), goal->get_global_position());

            bool skip = false;
            for (auto pway : unordered_open) {
                if (pway.waypoint == neighbor_pointer) {
                    if(new_cost > std::get<1>(neighbor)) {
                        skip = true;
                    }
                } 
            }
            if (skip) {
                continue;
            }

            came_from[neighbor_pointer] = cur;

            std::get<1>(neighbor) = new_cost;
            std::get<2>(neighbor) = heuristic_cost;
            double f = std::get<1>(neighbor) + std::get<2>(neighbor);
            open.push(PWaypoint(neighbor_pointer, f));
        }
    }
}   

bool AStar::nav_to_goal(Node3D* victim, Waypoint* dest, double delta) {
    float SPEED = 40;
    
    // Move victim incrementally toward the current target point
    Transform3D victim_transform = victim->get_transform();
    victim_transform.origin = victim->get_global_transform().origin.move_toward(dest->get_global_position(), delta * SPEED);
    victim->set_global_transform(victim_transform);

    // Check if victim has reached the target point
    if (victim_transform.origin.distance_to(dest->get_global_position()) < 5) {
        return true;
    }
    return false;
}
// Returns the Euclidian Distance heuristic between nodes A and B
double AStar::euclidean(Vector3 a, Vector3 b) {
    return std::sqrt(std::pow((b.x - a.x), 2) + std::pow((b.y - a.y),2) + std::pow((b.z - a.z),2));

}

double AStar::manhattan(Vector3 a, Vector3 b) {
    return std::abs(b.x - a.x) + std::abs(b.y - a.y) + std::abs(b.z - a.z);

}
