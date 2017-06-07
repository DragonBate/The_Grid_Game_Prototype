// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "Grid_GameState.h"
#include "Grid_Tile_Manager.h"


// Sets default values
AGrid_Tile_Manager::AGrid_Tile_Manager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootScene;
}

// Called when the game starts or when spawned
void AGrid_Tile_Manager::BeginPlay()
{

	Super::BeginPlay();
	
	AGrid_GameState* const MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AGrid_GameState>() : NULL;
		
	if (MyGameState != NULL)
	{
		MyGameState->GenerateGridID(this);
		if (GridManagerID != -1)
		{
			MyGameState->GenerateGrid(this);
		}
	}
	// ---------------------
	// ---------------------


}

// Called every frame
void AGrid_Tile_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

