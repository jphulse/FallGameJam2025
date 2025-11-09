// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplinePathActor.h"
#include "Enemy.generated.h"

UCLASS()
class TEAMCUBETHIRDPERSON_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfollowing")
	ASplinePathActor* splinePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfollowing")
	bool pathInterrupt = false;

	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintImplementableEvent, Category = "Teleportation")
	void afterTeleport();
	UFUNCTION(BlueprintImplementableEvent, Category = "Teleportation")
	void stopAllTimelines();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
