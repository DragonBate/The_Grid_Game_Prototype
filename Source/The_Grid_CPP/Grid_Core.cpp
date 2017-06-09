// Fill out your copyright notice in the Description page of Project Settings.

#include "The_Grid_CPP.h"
#include "Grid_GameState.h"
#include "Base_Minion_Character.h"
#include "Grid_Core.h"


// Sets default values
AGrid_Core::AGrid_Core()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Root"));
	RootComponent = CollisionComp;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Static Mesh Component"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->InitSphereRadius(100.0f);
	SphereComponent->SetCanEverAffectNavigation(false);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	// set up a notification for when this component overlaps something  
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AGrid_Core::OnOverlapBegin);
	
}

void AGrid_Core::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Core Test Func"));
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Core Overlap"));
		ABase_Minion_Character* TempMinion = Cast<ABase_Minion_Character>(OtherActor);
		if (TempMinion != nullptr)
		{
			AGrid_GameState* MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AGrid_GameState>() : NULL;
			if (MyGameState != nullptr)
			{
				MyGameState->DecreaseCoreHealth(TempMinion->Minion_CoreDamage);
			}
			OtherActor->Destroy();
		}
		
	}
}



// Called when the game starts or when spawned
void AGrid_Core::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrid_Core::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

