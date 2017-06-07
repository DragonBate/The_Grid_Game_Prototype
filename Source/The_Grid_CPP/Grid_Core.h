// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Grid_Core.generated.h"

UCLASS()
class THE_GRID_CPP_API AGrid_Core : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid_Core();

	USceneComponent* CollisionComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
