// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "TeamCubeThirdPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));

	portalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("portalTrigger"));
	portalTrigger->SetupAttachment(RootComponent);

	spawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("spawnPoint"));
	spawnPoint->SetupAttachment(RootComponent);

	portalTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	portalTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	portalTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	portalTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APortal::TeleportActor(AActor* ActorToTeleport) {


	if (!destination || !ActorToTeleport)
		return;

	// Get destination transform
	

	// Teleport actor to destination position and rotation
	UE_LOG(LogTemp, Display, TEXT(" Destination %s"), *destination->GetActorLocation().ToString());
	ActorToTeleport->SetActorLocation(destination->GetActorLocation());
	UE_LOG(LogTemp, Display, TEXT("Player location %s"), *ActorToTeleport->GetActorLocation().ToString());
	//ActorToTeleport->SetActorRotation(DestTransform.GetRotation());

	// Optional: handle velocity and camera for characters
	ACharacter* Character = Cast<ACharacter>(ActorToTeleport);
	if (Character)
	{
		

		// Update player control rotation
		APlayerController* PC = Cast<APlayerController>(Character->GetController());
		if (PC)
		{
			
		}
	}

	// If it�s your custom TeamCubeCharacter, update camera manually
	if (ATeamCubeThirdPersonCharacter* CubeChar = Cast<ATeamCubeThirdPersonCharacter>(ActorToTeleport))
	{
		
		CubeChar->GetCharacterMovement()->SetGravityDirection(destination->gravityDownDirection);
			
	}


}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		TeleportActor(OtherActor);
	}
}



