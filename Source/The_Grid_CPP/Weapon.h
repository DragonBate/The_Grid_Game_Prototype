// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.h"
#include "Base_Minion_Character.h"
#include "Weapon.generated.h"

class AThe_Grid_CPPCharacter;

#define TRACE_WEAPON ECC_GameTraceChannel1

UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		EBullet			UMETA(Displayname = "Bullet"),
		ESpread			UMETA(Displayname = "Spread"),
		EProjectile		UMETA(Displayname = "Projectile"),
	};
}

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(editDefaultsOnly, Category = Config)
		float Damage;

	UPROPERTY(editDefaultsOnly, Category = Ammo)
		int32 MaxAmmo;

	UPROPERTY(editDefaultsOnly, Category = Ammo)
		int32 MaxClip;

	UPROPERTY(editDefaultsOnly, Category = Config)
		float TimeBetweenShots;

	UPROPERTY(editDefaultsOnly, Category = Ammo)
		int32 ShotCost;

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
class THE_GRID_CPP_API AWeapon : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION()
		void Fire();
	
	void SetOwningPawn(AThe_Grid_CPPCharacter* NewOwner);
	void OnEquip();
	void OnUnEquip();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void Instant_Fire();

	UFUNCTION()
		virtual	void ProjectileFire();

	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	AThe_Grid_CPPCharacter* MyPawn;

	void AttachToPlayer();
	void DetachFromPlayer();

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(editDefaultsOnly, BlueprintReadWrite, Category = Config)
		TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(editDefaultsOnly, Category = Config)
		FWeaponData WeaponConfig;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = Collision)
		class UBoxComponent* CollisionComp;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = Config)
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(editDefaultsOnly, Category = Projectile)
		TSubclassOf<class ARocket> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentClip;
};
