// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ShotgunBase.h"
#include "Weapons/Magazines/ammoBoxBase.h"
#include "Weapons/Magazines/Ammo/AmmoBase.h"
#include "Weapons/Magazines/AmmoContainerComponent.h"

#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>



AShotgunBase::AShotgunBase()
{
	LatchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LatchMesh"));
	LatchMesh->SetupAttachment(WeaponMesh);
	LatchMesh->SetCollisionProfileName("NoCollision");
	LatchMesh->SetGenerateOverlapEvents(false);
	LatchMesh->SetNotifyRigidBodyCollision(false);
	
	AmmoContainer = CreateDefaultSubobject<UAmmoContainerComponent>(TEXT("AmmoContainer"));
}

void AShotgunBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (!bHasInternalAmmoStorage && AmmoContainer && LatchMesh)
	{
		AmmoContainer->DestroyComponent();
		LatchMesh->DestroyComponent();
	}
}

void AShotgunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotgunBase::LoadAmmo(AAmmoBase* AmmoToLoad)
{
	if (AmmoContainer && AmmoToLoad && AmmoToLoad->IsA(AmmoContainer->AmmoClass) && AmmoContainer->AddSingleRound())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MagInSound, MagazineHitBox->GetComponentLocation());
		
		if (AAmmoBoxBase* AmmoBox = Cast<AAmmoBoxBase>(AmmoToLoad->GetOwner()))
		{
			AmmoBox->ClearSpawnedAmmo();
		}else
		{
			AmmoToLoad->Destroy();
		}
	}
}

void AShotgunBase::MoveLatch()
{
	if (LatchMesh)
	{
		LatchMesh->SetRelativeRotation(LatchRotationOffset);
		
		FTimerHandle TH;
		GetWorldTimerManager().SetTimer(TH, this, &AShotgunBase::ResetLatch, LatchResetDelay, false);
	}
}

void AShotgunBase::ResetLatch()
{
	if (LatchMesh)
	{
		LatchMesh->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

bool AShotgunBase::LoadRound()
{
	if (!LoadedAmmo)
	{
		if (bHasInternalAmmoStorage)
		{
			if (AmmoContainer->RemoveSingleRound())
			{
				SpawnLoadedAmmo(AmmoContainer->AmmoClass);
				return SelectedFiringMode  != FM_Single;
			}
		}
		else
		{
			return Super::LoadRound();
		}
	}
	
	return false;
}

void AShotgunBase::OnMagazineBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (bHasInternalAmmoStorage)
	{
		if (AAmmoBase* AmmoToLoad = Cast<AAmmoBase>(OtherActor))
		{
			LoadAmmo(AmmoToLoad);
			MoveLatch();
		}
	}else
	{
		Super::OnMagazineBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, FromSweep, SweepResult);
	}
}

void AShotgunBase::StopMovingSlide()
{
	if (SelectedFiringMode == FM_Single)
	{
		bIsSlideBeingHeld = false;
	}else
	{
		Super::StopMovingSlide();
	}
}


