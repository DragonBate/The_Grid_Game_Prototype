// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Actor.h"
#include "Base_Minion_Character.h"
#include "Base_Turret.generated.h"

UCLASS()
class THE_GRID_CPP_API ABase_Turret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Turret();

	UPROPERTY(EditAnywhere)
		USceneComponent* TurretHead;

	UPROPERTY(EditAnywhere)
		float TurretScanTimeMax = 1;

	UPROPERTY(EditAnywhere)
		float TurretRotSpeed = 50;

	UPROPERTY(editDefaultsOnly, Category = Config)
		int32 TurretDamage;

	UPROPERTY(editDefaultsOnly, Category = Config)
		float TimeBetweenShots;

	UPROPERTY(editDefaultsOnly, Category = Config)
		float WeaponRange;

	UPROPERTY(editDefaultsOnly, Category = Config)
		float WeaponSpread;


	float TurretScanTimer = 1.0f;

	float TurretRateTimer = 1.0f;

	TArray<AActor*> FoundActors;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Filters Array of AActors*(Base_Enemy_Character) and outputs optimal Target
	UFUNCTION()
		AActor* FilterEnemies(TArray<AActor*> inputArray);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = Config)
		class UStaticMeshComponent* TurretMesh;
};
