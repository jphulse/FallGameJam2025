class_name Player extends CharacterBody3D


const SPEED = 5.0
const JUMP_VELOCITY = 4.5

@onready
var portalCooldown = $TeleportCoolDown
var canTeleport = true
var mouseSensitivity : float = 40
var deltaInput : float = 0


func _ready():
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED

func teleportPlayer(destination: Portal):
	if canTeleport:
		$Camera3D.rotation_degrees.x = 0
		$Camera3D.rotation_degrees.y = 0
		
		rotation_degrees.x = 0  # Prevent rolling
		rotation_degrees.z = 0  # Prevent unwanted rotations
		
		global_transform = destination.global_transform
		up_direction = -destination.global_transform.basis.y.normalized()
		transform = destination.transform
		print("Up Direction: ", up_direction)
		velocity = Vector3.ZERO
		
		
		canTeleport = false
		portalCooldown.start()
		print(global_position)
		print(destination.global_position)
		print(global_rotation)
		print(destination.global_rotation)
		print(up_direction)
		print(destination.global_basis)
		
func _input(event):
	if event is InputEventMouseMotion and Input.mouse_mode == Input.MOUSE_MODE_CAPTURED:
		#var mouse_x = event.relative.x * mouseSensitivity * deltaInput
		#var mouse_y = event.relative.y * mouseSensitivity * deltaInput
#
		## Apply horizontal rotation (rotate around Y axis)
		## Create a rotation matrix around the Y-axis (vertical axis)
		#var rotation_y = Transform3D().rotated(Vector3.UP, deg_to_rad(mouse_x))
#
		## Apply this rotation to the camera's local basis (apply to local space)
		#$Camera3D.transform.basis = rotation_y.basis * $Camera3D.transform.basis
#
		## Apply vertical rotation (rotate around X axis)
		## Create a rotation matrix around the X-axis (horizontal axis)
		#var rotation_x = Transform3D().rotated(Vector3.RIGHT, deg_to_rad(mouse_y))
#
		## Apply this rotation to the camera's local basis
		#$Camera3D.transform.basis = rotation_x.basis * $Camera3D.transform.basis
#
		## Optional: Clamp the camera's pitch (X-axis) to avoid flipping
		#var camera_rotation = $Camera3D.rotation_degrees
		#camera_rotation.x = clamp(camera_rotation.x, -70, 70)
		#$Camera3D.rotation_degrees = camera_rotation
		rotate_y(-event.relative.x * mouseSensitivity * deltaInput)
		$Camera3D.rotate_x(-event.relative.y * mouseSensitivity * deltaInput)
		$Camera3D.rotation.x = clampf($Camera3D.rotation.x, -deg_to_rad(70), deg_to_rad(70))
		

func _physics_process(delta: float) -> void:
	# Add the gravity.
	deltaInput = delta
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
	#var input_dir := Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	#var direction := (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	if Input.is_action_pressed("walkForward"):
		velocity = transform.basis.z.normalized() * -SPEED
	elif Input.is_action_pressed("Walk Back"):
		velocity = transform.basis.z.normalized() * SPEED
	elif Input.is_action_pressed("Walk Left"):
		velocity = transform.basis.x.normalized() * -SPEED
	elif Input.is_action_pressed("Walk RIght"):
		velocity = transform.basis.x.normalized() * SPEED
	#if direction != Vector3.ZERO:
		#velocity = direction * SPEED
	else:
		var local_velocity = transform.basis.inverse() * velocity
		local_velocity.x = move_toward(local_velocity.x, 0, SPEED * delta)
		local_velocity.z = move_toward(local_velocity.z, 0, SPEED * delta)
		velocity = transform.basis * local_velocity
	move_and_slide()


func _on_teleport_cool_down_timeout() -> void:
	canTeleport = true
