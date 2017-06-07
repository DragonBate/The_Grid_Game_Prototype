// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "GSFuncLib.h"


FVector UGSFuncLib::VectorRoundTo(FVector inputNumber, float inputSnapNumber)
{
	FVector Temp1;
	Temp1.X = FMath::GridSnap(inputNumber.X, inputSnapNumber);
	Temp1.Y = FMath::GridSnap(inputNumber.Y, inputSnapNumber);
	Temp1.Z = FMath::GridSnap(inputNumber.Z, inputSnapNumber);
	return Temp1;
}
