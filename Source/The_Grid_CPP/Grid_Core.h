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

protected:

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* CollisionComp;

};
