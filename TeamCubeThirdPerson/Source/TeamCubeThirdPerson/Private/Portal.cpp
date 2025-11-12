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
	portalTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
	/*portalTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	portalTrigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	portalTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);*/
	portalTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	portalTrigger->OnComponentEndOverlap.AddDynamic(this, &APortal::OnOverlapEnd);


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
	ATeamCubeThirdPersonCharacter* non = Cast<ATeamCubeThirdPersonCharacter>(ActorToTeleport);
	if (non) {
		if (!canTeleportPlayer) { // so hacky XD
			return;
		}

		destination->canTeleportPlayer = false;
		
	}


	if (!destination || !ActorToTeleport)
		return;

	
	AEnemy* enemy = Cast<AEnemy>(ActorToTeleport);
	// Enemies
	if (enemy) {
		UE_LOG(LogTemp, Display, TEXT("Teleporting enemy"));
		ActorToTeleport->SetActorLocation(destination->GetActorLocation());
		if (destination->splinePath) {
			UE_LOG(LogTemp, Display, TEXT("Sent a spline over"));
		}
		enemy->splinePath = destination->splinePath;
		enemy->pathInterrupt = true;
		enemy->stopAllTimelines();
		enemy->afterTeleport();
	}



	// Player character for now nothing else should be able to teleport besides the two listed here
	if (ATeamCubeThirdPersonCharacter* CubeChar = Cast<ATeamCubeThirdPersonCharacter>(ActorToTeleport))
	{
		ActorToTeleport->SetActorLocation(destination->GetActorLocation());
		CubeChar->GetCharacterMovement()->SetGravityDirection(destination->gravityDownDirection);

	}
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{	
	if (OtherActor && OtherActor != this &&!Cast<ASplinePathActor>(OtherActor))
	{
		TeleportActor(OtherActor);
	}
}

void APortal::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && Cast<ATeamCubeThirdPersonCharacter>(OtherActor))
	{
		canTeleportPlayer = true;
		UpdateVariableForTeleportPlayer();
	}
}