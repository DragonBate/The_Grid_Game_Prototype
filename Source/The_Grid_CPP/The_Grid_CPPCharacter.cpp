// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "The_Grid_CPP.h"
#include "The_Grid_CPPCharacter.h"
#include "The_Grid_CPPProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Grid_GameState.h"
#include "Engine.h"
#include "Engine/Blueprint.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AThe_Grid_CPPCharacter


AGrid_GameState* Player_MyGameState;


AThe_Grid_CPPCharacter::AThe_Grid_CPPCharacter()
{
	CurrentWeapon = NULL;

	Inventory.SetNum(3, false);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Ammo
	LoadedAmmo = 30;
	AmmoPool = 30;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AThe_Grid_CPPCharacter::OnCollision);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, -5.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P and FP_Gun
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void AThe_Grid_CPPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	Player_MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AGrid_GameState>() : NULL;

	// If GameState is Valid
	if (Player_MyGameState != nullptr)
	{
		TurretTable = Player_MyGameState->TurretTable;
	}

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	GiveDefaultWeapon();
}

void AThe_Grid_CPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Print Current Ammo Count in Weapon
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Purple, TEXT("Ammo Clip: ") + FString::FromInt(LoadedAmmo));
}

//////////////////////////////////////////////////////////////////////////
// Input

// On build / Right click
void AThe_Grid_CPPCharacter::BuildPiece()
{
	//location the PC is focused on
	const FVector Start = FirstPersonCameraComponent->RelativeLocation + GetActorLocation();

	//LineTraceDistance in facing direction of PC
	const FVector End = Start + GetControlRotation().Vector() * LineTraceDistance;

	//The trace data is stored here
	FHitResult HitData(ForceInit);

	//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, TEXT("No Trace"));
	//TRACE
	if (AThe_Grid_CPPCharacter::Trace(GetWorld(), this, Start, End, HitData, ECC_Pawn, false))
	{
		//Print out the name of the traced actor
		if (HitData.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString(HitData.GetActor()->GetName()));											//Print out Name of Hit Actor
			//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, TEXT("Trace Distance: ") + FString::SanitizeFloat(HitData.Distance));			//Print out distance from start of trace to impact point

			// If hit data is == to A Grid tile
			if (HitData.GetActor()->IsA(AGrid_Tile::StaticClass()))
			{
				FoundTile = HitData.GetActor();
			}
			else
			{
				FoundTile = nullptr;
			}

			if (FoundTile != nullptr)
			{
				if (TurretTable != nullptr && TurretTable->GetRowNames().IsValidIndex(SelectedBuilding))
				{
					FName TempSelected = TurretTable->GetRowNames()[SelectedBuilding];
					AGrid_Tile* CastedTile = Cast<AGrid_Tile>(FoundTile);
					if (CastedTile->Can_Place(TempSelected, Tile_CanBuy) == true && Tile_CanBuy == true)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Can be placed"));
						CastedTile->Build_Wall(TempSelected);
					}
				}
				
			}
		}
	}
}

void AThe_Grid_CPPCharacter::WallButton_1()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("wall"));
	SelectedBuilding = 0;
}

void AThe_Grid_CPPCharacter::MachineButton_2()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Machine Gun"));
	SelectedBuilding = 1;
}

void AThe_Grid_CPPCharacter::SniperButton_3()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Sniper"));
	SelectedBuilding = 2;
}

void AThe_Grid_CPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AThe_Grid_CPPCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AThe_Grid_CPPCharacter::OnReload);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AThe_Grid_CPPCharacter::NextWeapon);
	PlayerInputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AThe_Grid_CPPCharacter::PrevWeapon);
	PlayerInputComponent->BindAction("SpawnBuild", IE_Pressed, this, &AThe_Grid_CPPCharacter::BuildPiece);

	PlayerInputComponent->BindAction("Button_1", IE_Pressed, this, &AThe_Grid_CPPCharacter::WallButton_1);
	PlayerInputComponent->BindAction("Button_2", IE_Pressed, this, &AThe_Grid_CPPCharacter::MachineButton_2);
	PlayerInputComponent->BindAction("Button_3", IE_Pressed, this, &AThe_Grid_CPPCharacter::SniperButton_3);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThe_Grid_CPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThe_Grid_CPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThe_Grid_CPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThe_Grid_CPPCharacter::LookUpAtRate);

}

// Spawn Starting Weapon
void AThe_Grid_CPPCharacter::GiveDefaultWeapon()
{
	//Set Spawner
	AWeapon* Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
		if (Spawner)
		{			
			Inventory[Spawner->WeaponConfig.Priority] = Spawner;
			CurrentWeapon = Inventory[Spawner->WeaponConfig.Priority];
			CurrentWeapon->SetOwningPawn(this);
			CurrentWeapon->OnEquip();
		}
}

void AThe_Grid_CPPCharacter::FireWeapon()
{
	//Fire Weapon if theres a Weapon exist
	if (CurrentWeapon != NULL)
	{
		CurrentWeapon->Fire();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "No Weapon in hand");
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	/*
	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();

		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	*/
}

void AThe_Grid_CPPCharacter::NextWeapon()
{
	if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority !=2)
	{
		if (Inventory[CurrentWeapon->WeaponConfig.Priority + 1] == NULL)
		{
			for (int32 i = CurrentWeapon->WeaponConfig.Priority + 1; i < Inventory.Num(); i++)
			{
				if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass()))
				{
					EquipWeapon(Inventory[i]);
				}
			}
		}
		else
		{
			EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority + 1]);
		}
	}
	else
	{
		EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
	}
}

void AThe_Grid_CPPCharacter::PrevWeapon()
{
	if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != 0)
	{
		if (Inventory[CurrentWeapon->WeaponConfig.Priority - 1] == NULL)
		{
			for (int32 i = CurrentWeapon->WeaponConfig.Priority - 1; i >=0; i--)
			{
				if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass()))
				{
					EquipWeapon(Inventory[i]);
				}
			}
		}
		else
		{
			EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority - 1]);
		}
	}
	else
	{
		EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
	}
}

void AThe_Grid_CPPCharacter::EquipWeapon(AWeapon * Weapon)
{
	if (CurrentWeapon != NULL)
	{
		CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->OnUnEquip();
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this);
		Weapon->OnEquip();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, "Equip: " + FString(Weapon->WeaponConfig.Name));
	}
	else
	{
		CurrentWeapon = Weapon;
		CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->SetOwningPawn(this);
		Weapon->OnEquip();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, "Equip: " + FString(Weapon->WeaponConfig.Name));
	}
}

//On Weapon Pick up Add weapon To Inventory then Destroy Weapon Actor in world
void AThe_Grid_CPPCharacter::OnCollision(UPrimitiveComponent* HitComp, class AActor * OtherActor, class UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AWeapon* Weapon = Cast <AWeapon>(OtherActor);
	if (Weapon)
	{
		ProcessWeaponPickup(Weapon);
	}
}

// Weapon Pickup
void AThe_Grid_CPPCharacter::ProcessWeaponPickup(AWeapon * Weapon)
{
	if (Weapon != NULL)
	{
		if (Inventory[Weapon->WeaponConfig.Priority] == NULL)
		{
			AWeapon* Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
			if (Spawner)
			{
				Inventory[Spawner->WeaponConfig.Priority] = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, "You just picked up a " + Inventory[Spawner->WeaponConfig.Priority]->WeaponConfig.Name + " With " + FString::FromInt(Weapon->CurrentAmmo) + "Ammo" );
			}
			Weapon->Destroy();
		}
		else
		{
			if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo >= 0 && Weapon->CurrentAmmo <= (Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo - Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo))
			{
				Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo += Weapon->CurrentAmmo;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Added: " + FString(Weapon->WeaponConfig.Name) + " With " + FString::FromInt(Weapon->CurrentAmmo) + " Ammo ");
				Weapon->Destroy();
			}
			else
			{
				if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo > Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo)
				{
					Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo = Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo;
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Added: " + FString::FromInt(Weapon->CurrentAmmo)  + " Ammo to weapon " + FString(Weapon->WeaponConfig.Name));
				}
			}
		}
	}
}

void AThe_Grid_CPPCharacter::OnReload()
{
	//LoadedAmmo = AmmoPool; //Sets Loaded Ammo to amount in AmmoPool on Reload.

   //Include if you want limited Reload Ammo
   /*
		if (AmmoPool <= 0 || LoadedAmmo >= 30)
		{
		return;
		}

		if (AmmoPool < (30 - LoadedAmmo))
		{
		LoadedAmmo = LoadedAmmo + AmmoPool;
		//AmmoPool = 0;
		}

		else
		{
		//AmmoPool = AmmoPool - (30 - LoadedAmmo);
		//LoadedAmmo = 30;
		}
		*/
}


void AThe_Grid_CPPCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}


void AThe_Grid_CPPCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	/*
	if (TouchItem.bIsPressed == false)
	{
	return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
	OnFire();
	}
	TouchItem.bIsPressed = false;
	*/
}

void AThe_Grid_CPPCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AThe_Grid_CPPCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AThe_Grid_CPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThe_Grid_CPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AThe_Grid_CPPCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AThe_Grid_CPPCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AThe_Grid_CPPCharacter::EndTouch);
	}
	return bResult;
}


//LINE TRACE

bool AThe_Grid_CPPCharacter::Trace
(UWorld* World,
	AActor* ActorToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel = ECC_Pawn,
	bool ReturnPhysMat = false)
{
	if (!World)
	{
		return false;
	}

	FCollisionQueryParams TraceParams(FName(TEXT("VictoreCore Trace")), true, ActorToIgnore);

	//Params
	TraceParams.bTraceComplex = true;
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;
	TraceParams.AddIgnoredActor(ActorToIgnore);	//Ignore Actors

	//Re-initialize hit info
	HitOut = FHitResult(ForceInit);

	//Trace!
	World->LineTraceSingleByChannel
	(
		HitOut,				//result
		Start,				//start
		End,				//end
		CollisionChannel,	//collision channel
		TraceParams
	);

	//Hit any Actor?
	return (HitOut.GetActor() != NULL);
}
