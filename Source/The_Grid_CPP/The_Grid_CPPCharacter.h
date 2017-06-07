// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "RocketLauncher.h"
#include "Grid_Tile.h"
#include "The_Grid_CPPCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class AThe_Grid_CPPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
		TSubclassOf<class AWeapon> WeaponSpawn;

	AWeapon* CurrentWeapon;

public:
	AThe_Grid_CPPCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AThe_Grid_CPPProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint32 bUsingMotionControllers : 1;

	/** Line trace distance from start to finish */
	UPROPERTY(EditAnywhere, Category = LineTrace)
		float LineTraceDistance = 1000;

	/** Ammo in magazine*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int32 LoadedAmmo = 30;

	/** Ammo in Storage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		int32 AmmoPool = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float Health;

	//Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray <class AWeapon*> Inventory;

protected:

	/** Fires a projectile. */
	//void OnFire();

	//Reload weapon
	void OnReload();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	UFUNCTION()
		void FireWeapon();

	void NextWeapon();
	void PrevWeapon();
	void EquipWeapon(AWeapon* Weapon);

	UFUNCTION()
		void OnCollision(UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

		void ProcessWeaponPickup(AWeapon* Weapon);

		void GiveDefaultWeapon();
		
		AActor* FoundTile;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	* Configures input for touchscreen devices if there is a valid touch interface for doing so
	*
	* @param	InputComponent	The input component pointer to bind controls to
	* @returns true if touch controls were enabled.
	*/
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

	FORCEINLINE bool Trace(UWorld * World, AActor * ActorToIgnore, const FVector & Start, const FVector & End, FHitResult & HitOut, ECollisionChannel CollisionChannel, bool ReturnPhysMat);

public:

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = Collision)
		class UBoxComponent* CollisionComp;


};

