// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "Grid_Tile_Manager.h"
#include "Grid_Tile.h"
#include "GSFuncLib.h"
#include "Base_Turret.h"
#include "Grid_Core.h"
#include "Engine.h"
#include "Grid_GameState.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTurrretDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 T_Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 T_Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bisTurret;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABase_Turret> Turret_Class;

	FTurrretDataStruct()
	{
		T_Price = 0;
		T_Power = 1;
		bisTurret = false;
	}

};

USTRUCT(BlueprintType)
struct FMinionDataStruct: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Currency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Health;

	FMinionDataStruct()
	{
		M_Currency = 0;
		M_Speed = 500;
		M_Health = 100;
	}

};

UCLASS()
class THE_GRID_CPP_API AGrid_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AGrid_GameState();

	UPROPERTY()
		TArray<AGrid_Tile_Manager*> ManagerArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerCurrency = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 GridPower = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CoreHealth = 1;

	UPROPERTY()
		UDataTable* TurretTable;

	UPROPERTY()
		UDataTable* MinionTable;




	UFUNCTION()
		void GenerateGridID(AGrid_Tile_Manager* inputManager);

	UFUNCTION()
		void GenerateGrid(AGrid_Tile_Manager* inputManager);

	UFUNCTION(BlueprintCallable)
		int32 Add_To_Currency(int32 inputNum);

	UFUNCTION(BlueprintCallable)
		int32 Add_To_Power(int32 inputNum);

	UFUNCTION(BlueprintCallable)
		void DecreaseCoreHealth();


};
