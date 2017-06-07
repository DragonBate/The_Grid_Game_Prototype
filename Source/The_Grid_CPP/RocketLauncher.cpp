// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "RocketLauncher.h"

void ARocketLauncher::ProjectileFire()
{
	Super::ProjectileFire();
	
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FVector MFLoc = WeaponMesh->GetSocketLocation("MF");
			FRotator MFRot = WeaponMesh->GetSocketRotation("MF");
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ARocket* const Rocket = GetWorld()->SpawnActor<ARocket>(ProjectileClass, MFLoc, MFRot, SpawnParams);
			
		}
	}
	
}


