extends ColorRect

@export var rotation_speed: float = 90.0
@export var pause_duration: float = 2.0

var biomes = [$Lava, $Ocean, $Mountain, $Tundra]
var curr_visible = 0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	get_next()
	#make_visible(curr_visible)
	pass
	
func get_next():
	curr_visible += 1
	curr_visible = curr_visible % 4
	
