#ifndef DECISIONTREE_H
#define DECISIONTREE_H


#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/project_settings.hpp>


#include "TreeNode.h"
#include "DecisionTree.h"
#include "AgentAction.h"

// This is an external library!
// https://github.com/nlohmann/json/releases/tag/v3.11.3
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

using Action = AgentAction::actions;

namespace godot {

class DecisionTree : public Object {
	GDCLASS(DecisionTree, Object)

public:
	void _process(double delta);
	void _ready();

	DecisionTree();
	~DecisionTree();

	void create_decision_tree(String agent_spec_file);
	Action make_decision(TreeNode* cur, std::map<std::string, bool> state);
	void parse_json_helper(nlohmann::ordered_json blob, TreeNode* cur, bool is_right, bool is_root);

	TreeNode* root;

protected:
	static void _bind_methods();
};

}

#endif