class_name Player extends CharacterBody3D


const SPEED = 5.0
const JUMP_VELOCITY = 4.5

@onready
var portalCooldown = $TeleportCoolDown
var canTeleport = true


func teleportPlayer(destination: Portal):
	if canTeleport:
		global_transform = destination.global_transform
		up_direction = -global_transform.basis.y.normalized()
		canTeleport = false
		portalCooldown.start()
		print(global_position)
		print(global_rotation)
		print(up_direction)
		
	

func _physics_process(delta: float) -> void:
	# Add the gravity.
	if not is_on_floor():
		velocity += get_gravity().length() * delta * -up_direction.normalized()
		#print(velocity)
		#print(velocity, up_direction.normalized(), get_gravity().length())
	#else:
		#print(is_on_floor())

	# Handle jump.
	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
		velocity += up_direction.normalized() * JUMP_VELOCITY 


	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var input_dir := Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	var direction := (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	if direction != Vector3.ZERO:
		velocity = direction * SPEED
	else:
		var local_velocity = transform.basis.inverse() * velocity
		local_velocity.x = move_toward(local_velocity.x, 0, SPEED * delta)
		local_velocity.z = move_toward(local_velocity.z, 0, SPEED * delta)
		velocity = transform.basis * local_velocity
	move_and_slide()


func _on_teleport_cool_down_timeout() -> void:
	canTeleport = true
