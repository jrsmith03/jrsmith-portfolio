#include "DecisionTree.h"

using namespace godot;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;


/* 
    This algorithm is NOT going to be learned. We don't necessarily want a 
    learned agent for a game AI because we want to manually tune the decision
    modes for optimal gameplay. 

    This Decision Tree is created by an Agent and interfaced by its corresponding 
    Behavior Manager.
    The Agent must provide a Behavior Specification JSON file (see documentation).
    If the specification is invalid. the Decision Tree will return an action of NONE.
    An invalid specification may include providing a behavior sequence that is unreachable.
    Under-specified states will result in errors!
*/

void DecisionTree::_bind_methods() {
    ClassDB::bind_method(D_METHOD("create_decision_tree", "agent_definition"),
       &DecisionTree::create_decision_tree);

}

DecisionTree::DecisionTree() {
}

DecisionTree::~DecisionTree() {
}

void DecisionTree::create_decision_tree(String agent_definition) {
    String json_path = ProjectSettings::get_singleton()->globalize_path(agent_definition);
    std::ifstream f(json_path.utf8().get_data());
    if (!f) {
        UtilityFunctions::printerr("The specified behavior configuration file is invalid.");
    } else {
        ordered_json data = ordered_json::parse(f);
        parse_json_helper(data, nullptr, true, true);
    }
    // std::map<std::string, bool> test_state;
    // test_state["player_going_to_win"] = false;
    // test_state["high_score"] = false;
    // test_state["close_to_player"] = true;
    // test_state["on_bouncy_platform"] = true;
    // test_state["fast_speed"] = true;

    // make_decision(this->root, test_state);
}

void DecisionTree::parse_json_helper(ordered_json blob, TreeNode* prev, bool is_right, bool is_root) {
    AgentAction* my_agent = memnew(AgentAction); 

    for (auto iterator = blob.begin(); iterator  != blob.end(); ++iterator) {
        // Now we can iterate over the JSON blob
        // We want to extract out the String values of the Key, Value pairs
        
        // Create the new node for this given field
        TreeNode* node = memnew(TreeNode);
        
        std::string str = iterator.key();
        if (is_root) {
            this->root = node;
            prev = node;
        }
        node->name = str.c_str();
        UtilityFunctions::print("Node name: ", String(str.c_str()));
        UtilityFunctions::print("   is_root: ", is_root);
        UtilityFunctions::print("   is_right: ", is_right);
        UtilityFunctions::print("  parent: ", String(prev->name.c_str()));
        if (is_right) {
            prev->right_child = node;
        } else {
            prev->left_child = node;
        }

        if (!iterator.value().is_object()) {
            // Leaf node
            std::string val = iterator.value();
            UtilityFunctions::print("   it's a leaf");
            UtilityFunctions::print("     decision: ", String(val.c_str()));
            node->is_leaf = true;
            // Need to parse the action from the value

            node->return_action = my_agent->get_enum_from_json(val);
        } else {
            // We need to recursively treat the object value of the iterator's value as a new JSON blob
            // Due to the JSON ordering, the first node in the subtree will always be the right child.
            parse_json_helper(iterator.value(), node, true, false);
        }
        // Because we are moving to the next item on this tree level (due to ordering), the child will be left.
        is_right = !is_right;

    }
}


Action DecisionTree::make_decision(TreeNode* cur, std::map<std::string, bool> state) {
    // Base case
    if (cur->is_leaf) {
        return cur->return_action;
    }

    // Determine if we should traverse left or right based on State
    // Need to make sure that the state list contains the node we're traversing on!
    
    if (state.count(cur->name) != 0) {
        bool split_right = state[cur->name];
        if (split_right && cur->right_child) {
            return make_decision(cur->right_child, state);
        } else if (!split_right && cur->left_child) {
            return make_decision(cur->left_child, state);
        }
    } else {
        UtilityFunctions::printerr("Error: State does not correctly correspond to behavior specification!");
        // In this error case, do nothing.
        return Action::NONE;
    }
    return Action::NONE;

}
