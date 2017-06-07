// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "Grid_Core.h"


// Sets default values
AGrid_Core::AGrid_Core()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	RootComponent = CollisionComp;
}

// Called when the game starts or when spawned
void AGrid_Core::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrid_Core::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

