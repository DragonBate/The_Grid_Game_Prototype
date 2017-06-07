// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "The_Grid_CPPHUD.generated.h"

UCLASS()
class AThe_Grid_CPPHUD : public AHUD
{
	GENERATED_BODY()

public:
	AThe_Grid_CPPHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

