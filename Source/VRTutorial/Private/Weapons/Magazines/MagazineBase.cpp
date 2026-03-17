// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Magazines/MagazineBase.h"
#include "Weapons/Magazines/Ammo/AmmoBase.h"
#include "Weapons/Magazines/AmmoContainerComponent.h"

#include <Components/StaticMeshComponent.h>
#include <Components/InstancedStaticMeshComponent.h>

// Sets default values
AMagazineBase::AMagazineBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName("GrabbableObject");
	Mesh->SetSimulatePhysics(true);
	
	CarryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarryMesh"));
	CarryMesh->SetupAttachment(Mesh);
	CarryMesh->SetCollisionProfileName("NoCollision");
	
	AmmoContainer = CreateDefaultSubobject<UAmmoContainerComponent>(TEXT("AmmoContainer"));

}

// Called when the game starts or when spawned
void AMagazineBase::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateCarryPosition();
}

void AMagazineBase::UpdateCarryPosition()
{
	if (AmmoContainer->StoredAmmo)
	{
		FName CarryBoneName = FName("carry_bone" + FString::FromInt(AmmoContainer->StoredAmmo->GetInstanceCount()-1));
		
		if (Mesh->DoesSocketExist(CarryBoneName))
		{
			CarryMesh->SetRelativeTransform(Mesh->GetSocketTransform(CarryBoneName, RTS_Component));
		}
	}
}

void AMagazineBase::RefillMag()
{
	AmmoContainer->RefillAmmo();
	UpdateCarryPosition();
}

bool AMagazineBase::RemoveSingleRound()
{
	bool bWasRoundremoved = AmmoContainer->RemoveSingleRound();
	UpdateCarryPosition();
	return bWasRoundremoved;
}

void AMagazineBase::UpdateAllAmmoInstances()
{
	AmmoContainer->UpdateAllAmmoInstances();
}

int AMagazineBase::GetCurrentAmmoCount()
{
	return AmmoContainer->CurrentAmmoCount;
}

TSubclassOf<AAmmoBase> AMagazineBase::GetAmmoClass()
{
	return AmmoContainer->AmmoClass;
}



