// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "The_Grid_CPP.h"
#include "The_Grid_CPPGameMode.h"
#include "The_Grid_CPPHUD.h"
#include "The_Grid_CPPCharacter.h"

AThe_Grid_CPPGameMode::AThe_Grid_CPPGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AThe_Grid_CPPHUD::StaticClass();
}
