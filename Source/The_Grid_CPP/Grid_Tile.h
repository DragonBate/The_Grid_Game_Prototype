// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
//#include "Grid_Tile_Manager.h"
#include "GSFuncLib.h"
#include "Engine.h"
#include "Grid_Tile.generated.h"

UCLASS()
class THE_GRID_CPP_API AGrid_Tile : public AActor
{
	GENERATED_BODY()



public:
	// Sets default values for this actor's properties
	AGrid_Tile();

	UPROPERTY(EditAnywhere)
		TArray<int32> Adjacents;

	UPROPERTY(EditAnywhere)
		bool bisPathBlocked = false;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TileCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bcanRaise = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RaiseHeight = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bisRaised = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bplaceTurret = false;

	UPROPERTY(EditAnywhere)
		FVector OriginalLocation = FVector(0.0f);

	UPROPERTY(EditAnywhere)
		int32 GridManagerID = -1;

	UPROPERTY(EditAnywhere)
		FName SelectedTurretName = FName();


	UFUNCTION(BlueprintCallable)
		bool Can_Place(FName inputClass, bool inputCanBuy = false);

	UFUNCTION(BlueprintCallable)
		bool Build_Wall(FName inputName);

	UFUNCTION(BlueprintCallable)
		bool Get_Is_Turret(FName inputName);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
