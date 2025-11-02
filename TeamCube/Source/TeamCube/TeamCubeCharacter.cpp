// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeamCubeCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TeamCube.h"

ATeamCubeCharacter::ATeamCubeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	RootComponent = GetCapsuleComponent();

	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(GetRootComponent());

	
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(CameraRoot);
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(0.f, 0.f, CameraOffset), FRotator(0.f, 0.f, 0.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = false;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;
	GetCharacterMovement()->GravityScale = 0.f; // Disable gravity

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ATeamCubeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATeamCubeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATeamCubeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATeamCubeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATeamCubeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATeamCubeCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogTeamCube, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ATeamCubeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ATeamCubeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ATeamCubeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{

		FRotator CharacterRotation = GetActorRotation();

		// Build rotation axes relative to character orientation
		FVector RightVector = CharacterRotation.RotateVector(FVector::RightVector);
		FVector UpVector = CharacterRotation.RotateVector(FVector::UpVector);


		
		FQuat YawQuat(UpVector, FMath::DegreesToRadians(Yaw));
		FQuat PitchQuat(RightVector, FMath::DegreesToRadians(Pitch)); 

		// Combine rotations
		FQuat NewRotationQuat = PitchQuat * YawQuat;

		// Apply to controller rotation
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FRotator NewControlRotation = (NewRotationQuat * PC->GetControlRotation().Quaternion()).Rotator();
			PC->SetControlRotation(NewControlRotation);
			//FRotator RelativeCameraRot = PC->GetControlRotation() - CharacterRotation;

			//// Apply it to CameraRoot (or your camera component’s parent)
			//CameraRoot->SetRelativeRotation(RelativeCameraRot);
		}
		// pass the rotation inputs
		/*AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);*/
	}
}

void ATeamCubeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{


		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ATeamCubeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ATeamCubeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void ATeamCubeCharacter::UpdateCamera(FRotator target)
{
	//FirstPersonCameraComponent->SetRelativeLocation(FVector::ZeroVector);
	//target.Yaw += 180.f;

	CameraRoot->SetWorldRotation(target);

	// Get the player's world up vector
	FVector PlayerUp = GetActorUpVector();

	// Desired offset magnitude (e.g. 60 units above the head)
	float OffsetDistance = CameraOffset;

	// Offset vector in world space (above the player)
	FVector WorldOffset = PlayerUp * OffsetDistance;

	// Convert world offset to CameraRoot local space (relative)
	FVector LocalOffset = CameraRoot->GetComponentTransform().InverseTransformVectorNoScale(WorldOffset);

	// Set the relative location of the camera
	FirstPersonCameraComponent->SetRelativeLocation(LocalOffset);

	//FirstPersonCameraComponent->SetRelativeLocation(GetActorUpVector() * CameraOffset);
	//FVector PlayerForward = GetActorForwardVector();
	//FVector PlayerUp = GetActorUpVector();

	//// Build a rotation that makes the camera's up match the player's up
	//FRotator TargetCameraRotation = UKismetMathLibrary::MakeRotationFromAxes(
	//	PlayerForward,
	//	FVector::CrossProduct(PlayerUp, PlayerForward), // Right vector
	//	PlayerUp
	//);

	//FirstPersonCameraComponent->SetWorldRotation(TargetCameraRotation);
}

void ATeamCubeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector down = -GetActorUpVector();
	FVector gravity = down * gravityStrength * DeltaTime;

	FVector currentVelocity = GetCharacterMovement()->Velocity;
	currentVelocity += gravity;

	GetCharacterMovement()->Velocity = currentVelocity;


}

