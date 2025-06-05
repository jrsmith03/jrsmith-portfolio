#ifndef TREENODE_H
#define TREENODE_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/classes/engine.hpp>
#include "AgentAction.h"

#include <cstdlib>

namespace godot {

class TreeNode : public Object {
GDCLASS (TreeNode, Object)

	public: 
		TreeNode();
		~TreeNode();
		
		void print_tree(TreeNode* start);


		std::string name;
		bool is_leaf = false;
		TreeNode* left_child = nullptr;
		TreeNode* right_child = nullptr;
		AgentAction::actions return_action = AgentAction::actions::IDLE;

	protected:
		static void _bind_methods();

};

}
#endif
