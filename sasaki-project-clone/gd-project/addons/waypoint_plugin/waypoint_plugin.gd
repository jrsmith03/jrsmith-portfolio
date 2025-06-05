@tool
extends EditorInspectorPlugin

var editor_interface : EditorInterface
var editor_plugin : EditorPlugin
var current_waypoint : Waypoint
var neighbor_dialog : Window


signal open_dialog(current_waypoint)


func set_plugin(plugin):
	editor_plugin = plugin


func _can_handle(object):
	#print("object type is: ", object.get_class())
	return object.get_class() == "Waypoint"
	
	
func _parse_property(object, type, name, hint_type, hint_string, usage_flags, wide):
	#print("parse property: object class: ", object.get_class())
	#print(object.get_property_list())
	if object.get_class() == "Waypoint":
		current_waypoint = object
		if name == "neighbors":
			#print("waypoint_plugin: current waypoint is ", current_waypoint.name)
			# Provide a label field, for the neighbors property
			var label = Label.new()
			label.text = "Neighbors"
			add_custom_control(label)
			
			# display all neighbors
			var neighbors = object.get(name)
			print("waypoint_plugin: neighbors of ", object.name)
			for i in range(neighbors.size()):
				if neighbors[i]:
					print("waypoint_plugin: neighbor ", i, " = ", neighbors[i])
					var neighbor_label = Label.new()
					neighbor_label.text = neighbors[i]
					add_custom_control(neighbor_label)
				
			# Button to add a new neighbor
			var add_button = Button.new()
			add_button.text = "Add Neighbor"
			add_button.connect("pressed", _on_add_neighbor_pressed)
			add_custom_control(add_button)
			
			var reset_neighbors_button = Button.new()
			reset_neighbors_button.text = "Reset Neighbors"
			reset_neighbors_button.connect("pressed", _on_reset_neighbors_pressed)
			add_custom_control(reset_neighbors_button)
			
			return true
	return false
		
	
func _on_add_neighbor_pressed():
	emit_signal("open_dialog", current_waypoint)
	pass
	
func _on_reset_neighbors_pressed():
	print("waypoint_plugin: reset neighbors pressed")
	current_waypoint.reset_neighbors()
	current_waypoint.notify_property_list_changed()
	pass
