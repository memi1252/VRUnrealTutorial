// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Magazines/AmmoBoxBase.h"
#include "Weapons/Magazines/Ammo/AmmoBase.h"
#include "Weapons/Magazines/AmmoContainerComponent.h"

#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>

// Sets default values
AAmmoBoxBase::AAmmoBoxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName("GrabbableObject");
	Mesh->SetSimulatePhysics(true);
	
	AmmoContainer = CreateDefaultSubobject<UAmmoContainerComponent>(TEXT("AmmoContainer"));

}

AActor* AAmmoBoxBase::TakeAmmo(USkeletalMeshComponent* CharacterMesh, const FName& BoneToAttachTo)
{
	if (SpawnedAmmo)
	{
		return nullptr;
	}
	
	if (AmmoContainer->RemoveSingleRound())
	{
		FActorSpawnParameters ASP;
		ASP.Owner = this;
		ASP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnedAmmo = GetWorld()->SpawnActor<AAmmoBase>(AmmoContainer->AmmoClass, FTransform(), ASP);
		
		if (SpawnedAmmo)
		{
			FAttachmentTransformRules ATR(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			SpawnedAmmo->AttachToComponent(CharacterMesh, ATR, BoneToAttachTo);
			SpawnedAmmo->AmmoMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
			SpawnedAmmo->FiredAmmoMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
			return SpawnedAmmo;
		}
	}
	
	return nullptr;
}

void AAmmoBoxBase::ReturnAmmo()
{
	if (SpawnedAmmo)
	{
		AmmoContainer->AddSingleRound();
		ClearSpawnedAmmo();
	}
}

void AAmmoBoxBase::ClearSpawnedAmmo()
{
	if (SpawnedAmmo)
	{
		SpawnedAmmo->Destroy();
	}
	
	SpawnedAmmo = nullptr;
}

