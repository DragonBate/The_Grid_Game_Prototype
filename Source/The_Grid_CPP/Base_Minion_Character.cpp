// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "Minion_AI_Controller.h"
#include "Grid_Core.h"
#include "Grid_GameState.h"
#include "Base_Minion_Character.h"

AGrid_GameState* Enemy_MyGameState;
// Sets default values
ABase_Minion_Character::ABase_Minion_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->AIControllerClass = AMinion_AI_Controller::StaticClass();

	Mesh_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh_1->SetupAttachment(RootComponent);
}

void ABase_Minion_Character::DamageMinion(int32 inputDamage)
{
	Minion_Health -= inputDamage;
	UE_LOG(LogTemp, Warning, TEXT("Minion Take Damage"));
	if(Minion_Health <= 0)
	{
		if(Enemy_MyGameState != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Added Currency"));
			Enemy_MyGameState->Add_To_Currency(Minion_Currency);
		}
		this->Destroy();
	}

}

// Called when the game starts or when spawned
void ABase_Minion_Character::BeginPlay()
{
	Super::BeginPlay();

	Enemy_MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AGrid_GameState>() : NULL;

	// If GameState is Valid
	if(Enemy_MyGameState != nullptr)
	{
		// If Table in GameState is Valid
		if(Enemy_MyGameState->MinionTable != NULL)
		{
			FMinionDataStruct* TempData = Enemy_MyGameState->MinionTable->FindRow<FMinionDataStruct>(Minion_Name, TEXT("General"));
			// If TempData (Found Row) is Valid - If Row Exists
			if(TempData != nullptr)
			{
				Minion_Health = TempData->M_Health;
				Minion_Speed = TempData->M_Speed;
				Minion_Currency = TempData->M_Currency;
			}
		}
	}
	this->GetCharacterMovement()->MaxWalkSpeed = Minion_Speed;



	// Handles Moving AI Towards Core
	// Need to add event for when cant find core
	AMinion_AI_Controller* EnemyController = Cast<AMinion_AI_Controller>(GetController());
	// Enemy Controller Valid then Move to core
	if(EnemyController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy"));
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid_Core::StaticClass(), FoundActors);

		// First Found "Core" Valid then Move to core
		if(FoundActors.IsValidIndex(0) && FoundActors[0] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AI Move"));
			EnemyController->MoveToLocation(FoundActors[0]->GetActorLocation());
		}
	}
}

// Called every frame
void ABase_Minion_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase_Minion_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

