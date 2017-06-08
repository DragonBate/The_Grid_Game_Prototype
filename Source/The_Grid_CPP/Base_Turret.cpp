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
			if(TurretRateTimer >= TurretConfig.TimeBetweenShots)
			{
				TurretRateTimer = 0;
				ABase_Minion_Character* Enemy = Cast<ABase_Minion_Character>(TargetActor);
				if(Enemy != nullptr && IsValid(Enemy))
				{
					Enemy->DamageMinion(TurretConfig.TurretDamage);
				}
				// Fire();
			}
		}
	}
	;
}

void ABase_Turret::Fire()
{
	if (ProjectileType == ETurretProjectile::EBullet)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Bullet"));
		Instant_Fire();
	}

	if (ProjectileType == ETurretProjectile::ESpread)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Spread"));

		for (int32 i = 0; i <= TurretConfig.WeaponSpread; i++)
		{
			Instant_Fire();
		}
	}

	if (ProjectileType == ETurretProjectile::EProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Projectile"));
		ProjectileFire();

	}
}

void ABase_Turret::ProjectileFire()
{

}


AActor * ABase_Turret::FilterEnemies(TArray<AActor*> inputArray)
{
	float TempDist = TurretConfig.WeaponRange;
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

void ABase_Turret::Instant_Fire()
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = TurretConfig.WeaponSpread;
	const float SpreadCone = FMath::DegreesToRadians(TurretConfig.WeaponSpread * 0.5f);
	const FVector AimDir = TurretMesh->GetComponentRotation().Vector();
	const FVector StartTrace = TurretMesh->GetComponentLocation();
	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + ShootDir * TurretConfig.WeaponRange;
	const FHitResult Impact = TurretTrace(StartTrace, EndTrace);

	ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);

}
FHitResult ABase_Turret::TurretTrace(const FVector & TraceFrom, const FVector & TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceComplex = true;
	TraceParams.bTraceAsyncScene = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

void ABase_Turret::ProcessInstantHit(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir, int32 RandomSeed, float ReticleSpread)
{
	const FVector EndTrace = Origin + ShootDir * TurretConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Red, true, 0.5f, 10.0f);

	ABase_Minion_Character* Enemy = Cast<ABase_Minion_Character>(Impact.GetActor());
	if (Enemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("YOU HIT AN ENEMY!"));
		Enemy->Destroy();
	}
}



