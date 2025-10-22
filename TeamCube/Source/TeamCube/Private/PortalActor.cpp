// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalActor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APortalActor::APortalActor()
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
	portalTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortalActor::OnOverlapBegin);


}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();
	// Link with the other portal
	
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Eventually this should update the shader or texture thwe portal shows

}

void APortalActor::TeleportActor(AActor* ActorToTeleport) {
	if (!destination) {
		return;  // There is no destination portal, don't bother
	}

	FTransform EntityTransform = GetActorTransform();
	FTransform ExitTransform = destination->spawnPoint->GetComponentTransform();

	FTransform RelativeTransform = ActorToTeleport->GetActorTransform().GetRelativeTransform(EntityTransform);

	FTransform NewTransform = RelativeTransform * ExitTransform;

	ActorToTeleport->SetActorTransform(NewTransform);

	ACharacter* Character = Cast<ACharacter>(ActorToTeleport);
	if (Character) {
		FVector LocalVelocity = EntityTransform.InverseTransformVectorNoScale(Character->GetVelocity());
		FVector NewVelocity = ExitTransform.TransformVectorNoScale(LocalVelocity);

		Character->GetCharacterMovement()->Velocity = NewVelocity;
	}
}

void APortalActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		TeleportActor(OtherActor);
	}
}

