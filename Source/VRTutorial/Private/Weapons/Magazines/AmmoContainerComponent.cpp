// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/Magazines/AmmoContainerComponent.h"
#include "Weapons/Magazines/Ammo/AmmoBase.h"

#include <Components/StaticMeshComponent.h>
#include <Components/InstancedStaticMeshComponent.h>

// Sets default values for this component's properties
UAmmoContainerComponent::UAmmoContainerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	StoredAmmo = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StoredAmmo"));
	StoredAmmo->SetCollisionProfileName("NoCollision");
	StoredAmmo->SetNotifyRigidBodyCollision(false);
	StoredAmmo->SetGenerateOverlapEvents(false);
}

// Called when the game starts
void UAmmoContainerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner() && AmmoClass.GetDefaultObject())
	{
		if (UStaticMeshComponent* RootComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()))
		{
			MeshRef = RootComp;
		}
		
		StoredAmmo->AttachToComponent(MeshRef, FAttachmentTransformRules::SnapToTargetIncludingScale);
		StoredAmmo->SetStaticMesh(AmmoClass.GetDefaultObject()->AmmoMesh->GetStaticMesh());
		
		RefillAmmo();
	}
}

void UAmmoContainerComponent::RefillAmmo()
{
	CurrentAmmoCount = MaxAmmoCount;
	UpdateAllAmmoInstances();
}

void UAmmoContainerComponent::UpdateAllAmmoInstances()
{
	if (MeshRef && StoredAmmo)
	{
		StoredAmmo->ClearInstances();
		
		for (int i = 0; i < CurrentAmmoCount; i++)
		{
			FName AmmoBoneName = FName("ammo_bone_" + FString::FromInt(i));
			
			if (MeshRef->DoesSocketExist(AmmoBoneName))
			{
				StoredAmmo->AddInstance(MeshRef->GetSocketTransform(AmmoBoneName, RTS_Component));
			}
		}
	}
}

bool UAmmoContainerComponent::RemoveSingleRound()
{
	if (StoredAmmo && CurrentAmmoCount > 0)
	{
		CurrentAmmoCount--;
		
		if (CurrentAmmoCount < StoredAmmo->GetInstanceCount())
		{
			StoredAmmo->RemoveInstance(StoredAmmo->GetInstanceCount() - 1);
		}
		
		return true;
	}
	
	return false;
}

bool UAmmoContainerComponent::AddSingleRound()
{
	if (CurrentAmmoCount < MaxAmmoCount)
	{
		CurrentAmmoCount++;
		UpdateAllAmmoInstances();
		return true;
	}else
	{
		CurrentAmmoCount = MaxAmmoCount;
	}
	
	return false;
}





