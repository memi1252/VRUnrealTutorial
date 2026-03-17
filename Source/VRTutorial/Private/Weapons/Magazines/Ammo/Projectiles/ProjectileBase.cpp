// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Magazines/Ammo/Projectiles/ProjectileBase.h"
#include "Interfaces/DamageInterface.h"

#include <NiagaraSystem.h>
#include <Sound/SoundBase.h>
#include <NiagaraFunctionLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/DecalComponent.h>
#include <Materials/MaterialInstance.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->SetCollisionProfileName("Projectile");
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh -> OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);
	
	FHitResult Hit;
	FTimerDelegate TD;
	TD.BindUFunction(this, FName("DestroyProjectile"), Hit);
	GetWorldTimerManager().SetTimer(lifetimeTH, TD, Lifetime, false);
}

void AProjectileBase::DestroyProjectile(const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(lifetimeTH);
	
	if (Hit.bBlockingHit && Hit.GetComponent())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticles, Hit.Location, UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal));
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		
		if (UDecalComponent* SpawnedDecal = UGameplayStatics::SpawnDecalAttached(ImpactDecal, FVector(3.0f), Hit.GetComponent(), Hit.BoneName, Hit.Location,
			UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal), EAttachLocation::KeepWorldPosition, DecalLifetime))
		{
			SpawnedDecal->SetFadeScreenSize(DecalFadeScreenSize);
		}
	}else
	{
		if (bIsExplosive)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticles, GetActorLocation(), GetActorRotation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
		}
	}
	
	Destroy();
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IDamageInterface* DI = Cast<IDamageInterface>(OtherActor))
	{
		DI->Damage(DT_Projectile, Damage, Hit.BoneName);
	}
	
	DestroyProjectile(Hit);
}

