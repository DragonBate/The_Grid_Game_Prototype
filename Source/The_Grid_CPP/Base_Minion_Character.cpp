// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "Minion_AI_Controller.h"
#include "Grid_Core.h"
#include "Base_Minion_Character.h"


// Sets default values
ABase_Minion_Character::ABase_Minion_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->AIControllerClass = AMinion_AI_Controller::StaticClass();

	Mesh_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh_1->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABase_Minion_Character::BeginPlay()
{
	Super::BeginPlay();

	AMinion_AI_Controller* EnemyController = Cast<AMinion_AI_Controller>(GetController());

	if (EnemyController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy"));
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid_Core::StaticClass(), FoundActors);

		if (FoundActors.IsValidIndex(0) && FoundActors[0] != nullptr)
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

