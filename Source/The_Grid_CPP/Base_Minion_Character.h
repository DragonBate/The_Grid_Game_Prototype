// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Base_Minion_Character.generated.h"

UCLASS()
class THE_GRID_CPP_API ABase_Minion_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Minion_Character();

	UPROPERTY(editDefaultsOnly, Category = Config)
		FName Minion_Name = TEXT("Default");

	UPROPERTY(VisibleAnywhere, Category = Config)
		int32 Minion_Health = 100;

	UPROPERTY(VisibleAnywhere, Category = Config)
		float Minion_Speed = 500;

	UPROPERTY(VisibleAnywhere, Category = Config)
		int32 Minion_Currency = 1;

	UPROPERTY(VisibleAnywhere, Category = Config)
		int32 Minion_CoreDamage = 1;


	UFUNCTION(BlueprintCallable)
		void DamageMinion(int32 inputDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		class UStaticMeshComponent * Mesh_1;

};
