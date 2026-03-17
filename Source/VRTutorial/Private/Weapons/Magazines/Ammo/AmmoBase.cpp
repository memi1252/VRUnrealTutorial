// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Magazines/Ammo/AmmoBase.h"
#include "Weapons/Magazines/Ammo/Projectiles/ProjectileBase.h"

#include <Components/StaticMeshComponent.h>


// Sets default values
AAmmoBase::AAmmoBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	SetRootComponent(AmmoMesh);
	AmmoMesh->SetCollisionProfileName("GrabbableObject");

	FiredAmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FiredAmmoMesh"));
	FiredAmmoMesh->SetupAttachment(AmmoMesh);
	FiredAmmoMesh->SetCollisionProfileName("GrabbableObject");
	FiredAmmoMesh->SetVisibility(false);
}

void AAmmoBase::InitDespawnAmmo()
{
	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH, this, &AAmmoBase::DespawnAmmo, DespawnDelay, false);
}

void AAmmoBase::DespawnAmmo()
{
	Destroy();
}

void AAmmoBase::SetAmmoFired()
{
	bWasFired = true;
	AmmoMesh->SetVisibility(false);
	FiredAmmoMesh->SetVisibility(true);
}
