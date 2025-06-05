@tool
extends PopupPanel
class_name SelectWaypointDialog

@onready var waypoints_list : ItemList = $VBoxContainer/waypoints_list
@onready var select_button : Button = $VBoxContainer/select_button

@export var editor_plugin : EditorPlugin

var current_waypoint : Waypoint

signal neighbor_selected(selected_waypoint)

func set_plugin(plugin):
	editor_plugin = plugin

func _ready():
	title = "Select Neighbor for this Waypoint"
	min_size = Vector2(300, 400)
	
	#waypoints_list = ItemList.new()
	#waypoints_list.select_mode = ItemList.SELECT_SINGLE
	#add_child(waypoints_list)
	#
	#var confirm_button = Button.new()
	#confirm_button.text = "Confirm Selection"
	#confirm_button.connect("pressed", _on_confirm_button_pressed)
	#add_child(confirm_button)
	select_button.connect("pressed", _on_select_button_pressed)
	connect("close_requested", _on_close_requested)
	pass
	
func _on_close_requested():
	hide()
	
func populate_waypoint_list(waypoints_string, this_waypoint):
	current_waypoint = this_waypoint
	waypoints_list.clear()
	for waypoint in waypoints_string:
		if waypoint == this_waypoint.name:
			continue
		waypoints_list.add_item(waypoint)
	print("added waypoints to dialog list", waypoints_string)
	
func _on_select_button_pressed():
	var selected_index = waypoints_list.get_selected_items()
	print("dialog: selected index: ", selected_index)
	if selected_index.size() > 0:
		var selected_name = waypoints_list.get_item_text(selected_index[0])
		print("dialog: selected name: ", selected_name)
		var waypoints = get_tree().get_nodes_in_group("waypoints")
		print("dialog: waypoints: ", waypoints)
		var selected_waypoint = null
		for waypoint in waypoints:
			if waypoint.name == selected_name:
				selected_waypoint = waypoint
				break
				
		if selected_waypoint:
			emit_signal("neighbor_selected", selected_waypoint)
			print("emitted 'neighbor_selected' with waypoint: ", selected_waypoint.name)
	hide()
