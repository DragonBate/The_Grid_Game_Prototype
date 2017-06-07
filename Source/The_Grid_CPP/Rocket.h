// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Base_Minion_Character.h"
#include "Rocket.generated.h"

UCLASS()
class THE_GRID_CPP_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ARocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//NOTE TO SELF: In video he uses OnCollision instead of OnHit
	//On Hit with Other Actor
	UFUNCTION()
		void OnHit(UPrimitiveComponent * HitComp, class AActor * OtherActor, class UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movment)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		class UStaticMeshComponent * Mesh;

	
	
};
