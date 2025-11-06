// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalActor.generated.h"




UCLASS()
class TEAMCUBE_API APortalActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Portal")
	APortalActor* destination;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* portalTrigger;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* spawnPoint;

	UPROPERTY(EditAnywhere, Category = "Portal")
	FVector gravityDownDirection;
	
public:	
	// Sets default values for this actor's properties
	APortalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TeleportActor(AActor* ActorToTeleport);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

};
