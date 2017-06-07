// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "Grid_GameState.h"
#include "Base_Turret.h"
#include "Grid_Tile.h"


AGrid_GameState* MyGameState;
// Sets default values
AGrid_Tile::AGrid_Tile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	TileCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	TileCube->SetupAttachment(RootComponent);

	




	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Game/SM_Grid_Tile.SM_Grid_Tile"));
	if (CubeAsset.Succeeded())
	{
		TileCube->SetStaticMesh(CubeAsset.Object);
		TileCube->SetWorldScale3D(FVector(2.7f, 2.7f, 3.0f));
		TileCube->bCastDynamicShadow = false;

		//Create new material instance and assign it
		//	DynMat = UMaterialInstanceDynamic::Create(TileCube->GetMaterial(0), this);
		//	TileCube->SetMaterial(0, DynMat);
	}
}

bool AGrid_Tile::Can_Place(FName inputName, bool inputCanBuy)
{
	if(bisRaised == false)
	{
		// If GameState is Valid
		if (MyGameState != nullptr)
		{
			// If Table in GameState is Valid
			if( MyGameState->TurretTable != NULL )
			{
				FTurrretDataStruct* TempData = MyGameState->TurretTable->FindRow<FTurrretDataStruct>( inputName, TEXT( "General" ) );
				// If TempData (Found Row) is Valid - If Row Exists
				if(TempData != nullptr)
				{
					// If Power & Currency Wont go into Negatives
					if((MyGameState->GridPower - TempData->T_Power >= 0) && (MyGameState->PlayerCurrency - TempData->T_Price >= 0))
					{
						if(inputCanBuy == true)
						{
							MyGameState->GridPower -= TempData->T_Power;
							MyGameState->PlayerCurrency -= TempData->T_Price;
						}
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool AGrid_Tile::Build_Wall(FName inputName)
{
	if (bisRaised == true)
	{
		return false;
	}
	else
	{
		SelectedTurretName = inputName;
		bcanRaise = true;
		bisRaised = true;
		bplaceTurret = Get_Is_Turret(inputName);
		return true;
	}
	return false;
}

bool AGrid_Tile::Get_Is_Turret(FName inputName)
{
	// If GameState is Valid
	if(MyGameState != nullptr)
	{
		// If Table in GameState is Valid
		if(MyGameState->TurretTable != NULL)
		{
			FTurrretDataStruct* TempData = MyGameState->TurretTable->FindRow<FTurrretDataStruct>(inputName, TEXT("General"));
			// If TempData (Found Row) is Valid - If Row Exists
			if(TempData != nullptr)
			{
				// If Power & Currency Wont go into Negatives
				return TempData->bisTurret;
			}
		}
	}
	return false;
}






// Called when the game starts or when spawned
void AGrid_Tile::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = GetActorLocation();

	MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AGrid_GameState>() : NULL;
}

// Called every frame
void AGrid_Tile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bcanRaise == true)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, FMath::FInterpConstantTo(GetActorLocation().Z, OriginalLocation.Z + RaiseHeight, DeltaTime, 100)));
		if (FMath::IsNearlyEqual(GetActorLocation().Z, OriginalLocation.Z + RaiseHeight, 0.1f))
		{
			bcanRaise = false;
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y,OriginalLocation.Z + RaiseHeight));

			// If Set to Place Turret
			if (bplaceTurret == true)
			{
				// Log Place Turret
				UE_LOG(LogTemp, Warning, TEXT("Place Turret"));

				// If GameState is Valid
				if (MyGameState != nullptr)
				{
					// If Table in GameState is Valid
					if (MyGameState->TurretTable != NULL)
					{
						FTurrretDataStruct* TempData = MyGameState->TurretTable->FindRow<FTurrretDataStruct>(SelectedTurretName, TEXT("General"));
						// If TempData (Found Row) is Valid - If Row Exists
						if (TempData != nullptr)
						{
							FVector TargetLoc = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + TileCube->Bounds.BoxExtent.Z);
							
							ABase_Turret* TempTurret = GetWorld()->SpawnActor<ABase_Turret>(TempData->Turret_Class,TargetLoc,FRotator(0.0f, 0.0f, 0.0f));
						}
					}
				}
			}
		}
	}
}

