// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "GSFuncLib.generated.h"

/**
 * 
 */

USTRUCT()
struct FTurretOutputStruct
{
	GENERATED_BODY()

public:

	UPROPERTY()
		bool bIsTurret;
	UPROPERTY()
		bool bCanBuild;


	FTurretOutputStruct()
	{
		bIsTurret = false;
		bCanBuild = false;
	}

};


UCLASS()
class THE_GRID_CPP_API UGSFuncLib : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static FVector VectorRoundTo(FVector inputNumber, float inputSnapNumber);
};
