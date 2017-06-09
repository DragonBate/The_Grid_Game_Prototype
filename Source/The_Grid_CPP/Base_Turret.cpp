// Fill out your copyright notice in the Description page of Project Settings.
#include "The_Grid_CPP.h"
#include "Base_Minion_Character.h"
#include "Base_Turret.h"
#include "Engine.h"


// Sets default values
ABase_Turret::ABase_Turret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a dummy root component we can attach thing to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TurretHead = CreateDefaultSubobject<USceneComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzlepoint"));
	TurretMesh->SetupAttachment(TurretHead);
}

// Called when the game starts or when spawned
void ABase_Turret::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ABase_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurretScanTimer += DeltaTime;
	TurretRateTimer += DeltaTime;

	if(TurretScanTimer >= 1)
	{
		TurretScanTimer = 0;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABase_Minion_Character::StaticClass(), FoundActors);
	}

	AActor* TargetActor = FilterEnemies(FoundActors);

	if(TargetActor != nullptr)
	{

		FVector TempLoc = (TargetActor->GetActorLocation()) - (TurretHead->RelativeLocation + GetActorLocation());

		TurretHead->SetWorldRotation(FMath::RInterpTo(TurretHead->GetComponentRotation(), TempLoc.Rotation(),DeltaTime,TurretRotSpeed)	);



		// TempLoc.Rotation().Equals(TurretHead->GetComponentRotation(),5)
		// FVector::DotProduct((TargetActor->GetActorLocation()) - (TurretHead->RelativeLocation + GetActorLocation()), TurretHead->GetForwardVector()) >= 0.95

		// If Turret Angle close enough - Currently: 5 Degrees
		if(TempLoc.Rotation().Equals(TurretHead->GetComponentRotation(), 5))
		{
			// If Fire Rate Timer Has Reached Limit
			if(TurretRateTimer >= TimeBetweenShots)
			{
				TurretRateTimer = 0;
				ABase_Minion_Character* Enemy = Cast<ABase_Minion_Character>(TargetActor);
				if(Enemy != nullptr && IsValid(Enemy))
				{
					Enemy->DamageMinion(TurretDamage);
				}
			}
		}
	}
	;
}


AActor * ABase_Turret::FilterEnemies(TArray<AActor*> inputArray)
{
	float TempDist = WeaponRange;
	AActor* TempActor = nullptr;
		for(AActor* fActor : FoundActors)
		{
			FVector TempLoc = (fActor->GetActorLocation() - GetActorLocation());
			if(TempLoc.Size() < TempDist)
			{
				TempDist = TempLoc.Size();
				TempActor = fActor;
			}
		}
	return TempActor;
}


