#ifndef AGENT_ACTION_H
#define AGENT_ACTION_H


#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <map>
using namespace godot;
class AgentAction : public Object {
    GDCLASS(AgentAction, Object);
    public:

        enum actions { 
            IDLE , 
            FOLLOW,
            WANDER,
            PURSUIT,
            SHOOT,
            PUSH,
            RETREAT,
            PULL,
            PATHFIND,
            NONE
        };

        AgentAction::actions get_enum_from_json(std::string json_str);
        AgentAction();
        std::map<std::string, actions> str_to_action;
    
    protected:
        static void _bind_methods();
};

VARIANT_ENUM_CAST(AgentAction::actions);

#endif