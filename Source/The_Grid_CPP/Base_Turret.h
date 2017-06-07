// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.h"
#include "Base_Minion_Character.h"
#include "Base_Turret.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

UENUM(BlueprintType)
namespace ETurretProjectile
{
	enum ProjectileType
	{
		EBullet			UMETA(Displayname = "Bullet"),
		ESpread			UMETA(Displayname = "Spread"),
		EProjectile		UMETA(Displayname = "Projectile"),
	};
}

USTRUCT()
struct FTurretData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(editDefaultsOnly, Category = Config)
		float Damage;

	UPROPERTY(editDefaultsOnly, Category = Config)
		float TimeBetweenShots;

	UPROPERTY(editDefaultsOnly, Category = Config)
		float WeaponRange;

	UPROPERTY(editDefaultsOnly, Category = Config)
		float WeaponSpread;

	UPROPERTY(editDefaultsOnly, Category = Config)
		FString Name;

	UPROPERTY(editDefaultsOnly, Category = Config)
		int Priority;
};

UCLASS()
class THE_GRID_CPP_API ABase_Turret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Turret();

	UFUNCTION()
		void Fire();

	UPROPERTY(EditAnywhere)
		USceneComponent* TurretHead;

	UPROPERTY(EditAnywhere)
		float TurretScanTimeMax = 1;

	UPROPERTY(EditAnywhere)
		float TurretRotSpeed = 50;

	float TurretScanTimer = 1.0f;

	float TurretRateTimer = 1.0f;

	TArray<AActor*> FoundActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void Instant_Fire();

	UFUNCTION()
		virtual	void ProjectileFire();

	FHitResult TurretTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(editDefaultsOnly, BlueprintReadWrite, Category = Config)
		TEnumAsByte<ETurretProjectile::ProjectileType> ProjectileType;

	UPROPERTY(editDefaultsOnly, Category = Config)
		FTurretData TurretConfig;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = Config)
		class UStaticMeshComponent* TurretMesh;

	UPROPERTY(editDefaultsOnly, Category = Projectile)
		TSubclassOf<class ARocket> ProjectileClass;

	bool Shoot;

};
