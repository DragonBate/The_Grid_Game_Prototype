// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GSFuncLib.h"
#include "Grid_Tile.h"
#include "Grid_Tile_Manager.generated.h"


USTRUCT()
struct FGridArrayStruct
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TArray<AGrid_Tile*> GridArray1D;

	FGridArrayStruct()
	{
		GridArray1D = {};
	}

};

UCLASS()
class THE_GRID_CPP_API AGrid_Tile_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid_Tile_Manager();

	// Declare Vars
	UPROPERTY(EditAnywhere, Category = "Components")
		USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
		FVector GridEnd3D;
	
	UPROPERTY(EditAnywhere, Category = "2D Array")
		TArray<FGridArrayStruct> GridArray2D = {};

	UPROPERTY(EditAnywhere, Category = "Grid Data")
		float GridScaleSize = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid Data")
		int32 GridManagerID = -1;

	UPROPERTY(EditAnywhere, Category = "Grid Data")
		int32 GridManagerOrder = -1;


	// Declare Functions

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
