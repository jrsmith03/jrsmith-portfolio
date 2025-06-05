@tool
extends EditorPlugin

var waypoint_plugin

var neighbor_dialog 

var current_waypoint : Waypoint

func _enter_tree() -> void:
	# Initialization of the plugin goes here.
	waypoint_plugin = preload("res://addons/waypoint_plugin/waypoint_plugin.gd").new()
	waypoint_plugin.set_plugin(self)
	waypoint_plugin.editor_interface = get_editor_interface()
	add_inspector_plugin(waypoint_plugin)
	waypoint_plugin.connect("open_dialog", open_neighbor_dialog)
	
	# select dialog
	var dialog_scene = preload("res://addons/waypoint_plugin/select_waypoint_dialog.tscn")
	neighbor_dialog = dialog_scene.instantiate()
	neighbor_dialog.hide()
	neighbor_dialog.connect("neighbor_selected", _on_neighbor_selected)
	add_child(neighbor_dialog)
	# fetch the waypoints
	pass


func _exit_tree() -> void:
	# Clean-up of the plugin goes here.
	if neighbor_dialog:
		neighbor_dialog.queue_free()
	remove_inspector_plugin(waypoint_plugin)
	pass

func open_neighbor_dialog(this_waypoint):
	var waypoints = get_tree().get_nodes_in_group("waypoints")
	var waypoints_string = []
	for index in range(waypoints.size()):
		waypoints_string.append(waypoints[index].name)
		
	print("opening neighbor dialog for waypoint ", this_waypoint.name)
	current_waypoint = this_waypoint
	neighbor_dialog.populate_waypoint_list(waypoints_string, current_waypoint)
	neighbor_dialog.popup_centered()
	
			
func _on_neighbor_selected(neighbor_waypoint):
	print("waypoint_plugin: on neighbor selected called for waypoint: ", current_waypoint.name, " with neighbor waypoint: ", neighbor_waypoint.name)

	var waypoints = get_tree().get_nodes_in_group("waypoints")
	if current_waypoint and neighbor_waypoint:
		current_waypoint.call("add_neighbor", neighbor_waypoint.name)
		current_waypoint.notify_property_list_changed()
		print("added ", neighbor_waypoint.name, " as child of ", current_waypoint.name)
	pass
