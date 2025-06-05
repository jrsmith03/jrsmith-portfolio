#include "AgentAction.h"
using namespace godot;

void AgentAction::_bind_methods() {
    BIND_ENUM_CONSTANT(actions::IDLE);
    BIND_ENUM_CONSTANT(actions::FOLLOW);
    BIND_ENUM_CONSTANT(actions::WANDER);
    BIND_ENUM_CONSTANT(actions::PURSUIT);
    BIND_ENUM_CONSTANT(actions::SHOOT);
    BIND_ENUM_CONSTANT(actions::PUSH);
    BIND_ENUM_CONSTANT(actions::RETREAT);
    BIND_ENUM_CONSTANT(actions::PULL);
    BIND_ENUM_CONSTANT(actions::NONE);
}
AgentAction::AgentAction() {
        str_to_action = {
            {"IDLE", IDLE},
            {"WANDER", WANDER},
            {"PURSUIT", PURSUIT},
            {"SHOOT", SHOOT},
            {"PUSH", PUSH},
            {"PULL", PULL},
            {"DEFENSE", PULL},
            {"PATHFIND", PATHFIND},
            {"NONE", NONE}
        };
    }


AgentAction::actions AgentAction::get_enum_from_json(std::string json_str) {
    if (str_to_action.count(json_str) == 0) {
        UtilityFunctions::printerr("Error: Invalid Behavior Specification - returned action does not exist.");
        return actions::NONE;
    } else {
        return str_to_action[json_str];
    }
}