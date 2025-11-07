class_name Portal extends Area3D

@export 
var destination : Portal

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	if not destination:
		print("WARNING no destination")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _on_body_entered(body: Node3D) -> void:
	if body is Player and destination:
		body.teleportPlayer(destination)
	pass # Replace with function body.
