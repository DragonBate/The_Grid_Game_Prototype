// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "Grid_GameState.h"


AGrid_GameState::AGrid_GameState()
{
	ConstructorHelpers::FObjectFinder<UDataTable> TurretTable_BP(TEXT("DataTable'/Game/DataTables/Turret_DT.Turret_DT'"));
	if (TurretTable_BP.Object != nullptr)
	{
		TurretTable = TurretTable_BP.Object;
	}
}

void AGrid_GameState::GenerateGridID(AGrid_Tile_Manager * inputManager)
{
	ManagerArray.Add(inputManager);
	inputManager->GridManagerID = ManagerArray.IndexOfByKey(inputManager);
}

void AGrid_GameState::GenerateGrid(AGrid_Tile_Manager* inputManager)
{
	float TempGridSize = inputManager->GridScaleSize;
	FVector SnappedStartLoc = UGSFuncLib::VectorRoundTo(inputManager->GetActorLocation(), TempGridSize);
	FVector SnappedEndLoc = UGSFuncLib::VectorRoundTo(inputManager->GridEnd3D, TempGridSize);
	
	inputManager->GridArray2D.SetNum(FMath::RoundHalfToEven(SnappedEndLoc.Y / TempGridSize) + 1);

	// Y Axis Spawn Tiles
	for (int forY = 0; forY < FMath::RoundHalfToEven(SnappedEndLoc.Y / TempGridSize) + 1; forY++)
	{
		// X Axis Spawn Tiles
		for (int forX = 0; forX < FMath::RoundHalfToEven(SnappedEndLoc.X / TempGridSize) + 1; forX++)
		{
			// Get Location For Tile
			FVector TempLoc = FVector(SnappedStartLoc.X + (forX * TempGridSize), SnappedStartLoc.Y + (forY * TempGridSize), SnappedStartLoc.Z /* - (TempGridSize / 2) */);

			// Spawn Grid Tile
			AGrid_Tile* TempGridTile = GetWorld()->SpawnActor<AGrid_Tile>(TempLoc, FRotator(0.0f, 0.0f, 0.0f));

			// Add Grid Managers ID To Tile
			TempGridTile->GridManagerID = inputManager->GridManagerID;

			// Add Tile To Array
			inputManager->GridArray2D[forY].GridArray1D.Add(TempGridTile);
		}
	}
}

int32 AGrid_GameState::Add_To_Currency(int32 inputNum)
{
	PlayerCurrency = PlayerCurrency + inputNum;
	return PlayerCurrency;
}

int32 AGrid_GameState::Add_To_Power(int32 inputNum)
{
	GridPower = GridPower + inputNum;
	return GridPower;
}

