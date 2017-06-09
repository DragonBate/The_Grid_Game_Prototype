// Fill out your copyright notice in the Description page of Project Settings.
#include "The_Grid_CPP.h"
#include "Weapon.h"
#include "The_Grid_CPPCharacter.h"
#include "Engine.h"

//https://youtu.be/gJJCkLrK3gs?t=3780

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	if (ProjectileType == EWeaponProjectile::EBullet)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Bullet"));
		Instant_Fire();
	}

	if (ProjectileType == EWeaponProjectile::ESpread)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Spread"));

		for (int32 i = 0; i <= WeaponConfig.WeaponSpread; i++)
		{
			Instant_Fire();
		}
	}

	if (ProjectileType == EWeaponProjectile::EProjectile)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Projectile"));
		ProjectileFire();

	}
}

void AWeapon::Instant_Fire()
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = WeaponConfig.WeaponSpread;
	const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5f);
	const FVector AimDir = WeaponMesh->GetSocketRotation("MF").Vector();
	const FVector StartTrace = WeaponMesh->GetSocketLocation("MF");
	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
}

void AWeapon::ProjectileFire()
{

}

FHitResult AWeapon::WeaponTrace(const FVector & TraceFrom, const FVector & TraceTo) const
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

void AWeapon::ProcessInstantHit(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir, int32 RandomSeed, float ReticleSpread)
{
	const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Red, true, 0.5f, 10.0f);

	ABase_Minion_Character* Enemy = Cast<ABase_Minion_Character>(Impact.GetActor());
	if (Enemy)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("YOU HIT AN ENEMY!"));
		Enemy->DamageMinion(WeaponConfig.Damage);
	}
}

void AWeapon::SetOwningPawn(AThe_Grid_CPPCharacter * NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::AttachToPlayer()
{
	if (MyPawn)
	{
		//Detach Previous Weapon
		DetachFromPlayer();
		
		USkeletalMeshComponent* Character = MyPawn->GetMesh1P();

		//Attach New Weapon
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->AttachToComponent(Character, FAttachmentTransformRules::KeepRelativeTransform, "GripPoint");
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Attach " + FString(WeaponConfig.Name) + " To Player "); 
	}
}

void AWeapon::DetachFromPlayer()
{
	if (MyPawn)
	{
		//Detach Weapon from player
		WeaponMesh->SetHiddenInGame(true);
		WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Detach " + FString(WeaponConfig.Name) + " From Player ");
	}
}

void AWeapon::OnEquip()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToPlayer();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("OnEquip"));

}

void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("OnUnEquip"));

}
