// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class THE_GRID_CPP_API ARocketLauncher : public AWeapon
{
	GENERATED_BODY()
	
		virtual void ProjectileFire() override;
	
	
};
