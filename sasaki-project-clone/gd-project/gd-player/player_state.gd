extends Node3D

@export var finishedRace : bool
@export var lastCheckpointId : int
@export var startTime : float
@export var endTime : float
@export var laps : int

var last_checkpoint_position : Vector3

# Called when the player respawns
func respawn():
	global_transform.origin = last_checkpoint_position
	print("Player respawned at checkpoint:", lastCheckpointId)


func _on_death() -> void:
	respawn()

func update_checkpoint_id(new_id: int, checkpoint_position: Vector3) -> void:
	lastCheckpointId = new_id
	last_checkpoint_position = checkpoint_position
	print("Updated to checkpoint:", lastCheckpointId, "Position:", last_checkpoint_position)

func get_checkpoint_id() -> int:
	return lastCheckpointId

func get_laps() -> int:
	return laps

func increment_laps() -> void:
	laps += 1
	print("Laps:", laps)
