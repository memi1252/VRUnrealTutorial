// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponBase.h"
#include "VRCharacter.h"
#include "Weapons/Magazines/MagazineBase.h"
#include "Weapons/Magazines/Ammo/AmmoBase.h"
#include "Weapons/Magazines/Ammo/Projectiles/ProjectileBase.h"

#include <NiagaraSystem.h>
#include <Sound/SoundBase.h>
#include <NiagaraFunctionLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/StaticMeshComponent.h>
#include <PhysicsEngine/PhysicsConstraintComponent.h>

#include "MotionControllerComponent.h"


AProjectileWeaponBase::AProjectileWeaponBase()
{
	SlideMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlideMesh"));
	SlideMesh->SetupAttachment(WeaponMesh);
	SlideMesh->SetCollisionProfileName("GrabbableObject");
	
	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(WeaponMesh);
	TriggerMesh->SetCollisionProfileName("NoCollision");
	
	SelectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectorMesh"));
	SelectorMesh->SetupAttachment(WeaponMesh);
	SelectorMesh->SetCollisionProfileName("NoCollision");
	
	MagazineConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("MagazineConstraint"));
	MagazineConstraint->SetupAttachment(WeaponMesh, MagazineBone);
	MagazineConstraint->SetLinearXLimit(LCM_Locked, 0.0f);
	MagazineConstraint->SetLinearYLimit(LCM_Locked, 0.0f);
	MagazineConstraint->SetLinearZLimit(LCM_Locked, 0.0f);
	MagazineConstraint->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	MagazineConstraint->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	MagazineConstraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
	MagazineConstraint->SetDisableCollision(true);
	
	MagazineHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MagazineHitBox"));
	MagazineHitBox->SetupAttachment(WeaponMesh, MagazineBone);
	MagazineHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	MagazineHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	MagazineHitBox->SetGenerateOverlapEvents(true);
	MagazineHitBox->SetNotifyRigidBodyCollision(false);
	MagazineHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MagazineHitBox->SetCollisionObjectType(ECC_WorldDynamic);
	
}

void AProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (FiringModes.Num() <= 1)
	{
		SelectorMesh->DestroyComponent();
	}
	
	MagazineHitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileWeaponBase::OnMagazineBeginOverlap);
}

void AProjectileWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (SelectedFiringMode == FM_Tripple)
	{
		if (CurrentFiringCount < MaxFiringCount)
		{
			if (Fire())
			{
				CurrentFiringCount++;
			}
		}else
		{
			SetActorTickEnabled(false);
		}
	}else
	{
		Fire();
	}
}

bool AProjectileWeaponBase::Fire()
{
	if (GetWorld()->TimeSince(LastFireTime) >= FireRate)
	{
		LastFireTime = GetWorld()->GetTimeSeconds();
		
		if (IsSlideReleased())
		{
			if (LoadedAmmo && !LoadedAmmo->bWasFired)
			{
				LoadedAmmo->SetAmmoFired();
				
				FActorSpawnParameters ASP;
				ASP.Owner = this;
				ASP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				
				FTransform MuzzleTransform = WeaponMesh->GetSocketTransform(MuzzleBone);
				
				for (int i = 0; i < ProjectileCount; i++)
				{
					if (i == 0)
					{
						GetWorld()->SpawnActor<AProjectileWeaponBase>(LoadedAmmo->ProjectileClass, MuzzleTransform, ASP);
					}else
					{
						GetWorld()->SpawnActor<AProjectileBase>(LoadedAmmo->ProjectileClass,
							FTransform(
								FRotator(
									MuzzleTransform.Rotator().Pitch + FMath::RandRange(-MaxProjectileSpread, MaxProjectileSpread),
									MuzzleTransform.Rotator().Yaw + FMath::RandRange(-MaxProjectileSpread, MaxProjectileSpread),
									MuzzleTransform.Rotator().Roll),
									MuzzleTransform.GetLocation(),
									MuzzleTransform.GetScale3D()), ASP);
					}
				}
				
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlash, MuzzleTransform.GetLocation(), MuzzleTransform.Rotator());
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FiringSound, MuzzleTransform.GetLocation());
				
				ApplyRecoil();
				
				if (SelectedFiringMode != FM_Single && !bIsSlideBeingHeld)
				{
					MoveSlideBack();
					EjectShell();
					
					if (LoadRound())
					{
						FTimerHandle TH;
						GetWorldTimerManager().SetTimer(TH, this, &AProjectileWeaponBase::ReleaseSlide, FireRate / 2.0f, false);
					}else
					{
						bIsSlideStuck = true;
					}
				}else
				{
					bIsSlideStuck = true;
				}
				
				return true;
			}
		}else
		{
			if (bIsSlideStuck)
			{
				if (LoadRound())
				{
					ReleaseSlide();
				}
			}
		}
	}
	
	return false;
}

void AProjectileWeaponBase::EjectShell()
{
	if (LoadedAmmo)
	{
		LoadedAmmo->SetActorEnableCollision(true);
		LoadedAmmo->AmmoMesh->SetCollisionProfileName("GrabbableObject");
		LoadedAmmo->AmmoMesh->SetSimulatePhysics(true);
		LoadedAmmo->AmmoMesh->AddImpulse(WeaponMesh->GetComponentRotation().RotateVector(ShellEjectTrajectory) * ShellEjectForce, NAME_None, true);
		LoadedAmmo->InitDespawnAmmo();
		LoadedAmmo = nullptr;
	}
}

void AProjectileWeaponBase::MoveSlideBack()
{
	SlideMesh->SetRelativeLocation(FVector(SlideTraveDistance, 0.0f, 0.0f));
	
	if (!bWasSlidePulled)
	{
		bWasSlidePulled = true;
		bWasSlideReleased = false;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlideBackSound, SlideMesh->GetComponentLocation());
	}
}

void AProjectileWeaponBase::ReleaseSlide()
{
	SlideMesh->SetRelativeLocation(FVector::ZeroVector);
	bIsSlideStuck = false;
	
	if (bWasSlidePulled && !bWasSlideReleased)
	{
		bWasSlideReleased = true;
		bWasSlidePulled = false;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlideReleaseSound, SlideMesh->GetComponentLocation());
	}
}

EMagCheckReturnType AProjectileWeaponBase::CheckMag()
{
	if (LoadedMagazine && !MagEjectTimerHandle.IsValid())
	{
		if (LoadedMagazine->GetCurrentAmmoCount() > 0)
		{
			return MCRT_NotEmpty;
		}
		
		return MCRT_Empty;
	}
	
	return MCRT_NotAttached;
}

EAmmoCheckReturnType AProjectileWeaponBase::CheckAmmo()
{
	if (!LoadedAmmo)
	{
		switch (CheckMag())
		{
		case MCRT_NotAttached:
			break;
		case MCRT_NotEmpty:
			if (LoadedMagazine->RemoveSingleRound())
			{
				if (SpawnLoadedAmmo(LoadedMagazine->GetAmmoClass()))
				{
					return ACRT_Success;
				}
			}
			
			break;
		case MCRT_Empty:
			return ACRT_MagEmpty;
		}
	}
	
	return ACRT_Failed;
}

bool AProjectileWeaponBase::SpawnLoadedAmmo(TSubclassOf<AAmmoBase> AmmoClass)
{
	if (AmmoClass.Get())
	{
		FActorSpawnParameters ASP;
		ASP.Owner = this;
		ASP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		LoadedAmmo = GetWorld()->SpawnActor<AAmmoBase>(AmmoClass, SlideMesh->GetSocketTransform(LoadedAmmoBone), ASP);
		
		if (LoadedAmmo)
		{
			FAttachmentTransformRules ATR(EAttachmentRule::SnapToTarget, true);
			LoadedAmmo->AttachToComponent(SlideMesh, ATR, LoadedAmmoBone);
			LoadedAmmo->SetActorEnableCollision(false);
			return true;
		}
	}
	return false;
}

bool AProjectileWeaponBase::LoadRound()
{
	switch (CheckAmmo())
	{
		case ACRT_Success:
		return true;
	case ACRT_Failed:
		SetActorTickEnabled(false);
		return true;
	case ACRT_MagEmpty:
		SetActorTickEnabled(false);
		break;
	}
	
	return false;
}

bool AProjectileWeaponBase::IsSlidePulled()
{
	return SlideMesh->GetRelativeLocation().X <= SlideTraveDistance + 0.5f;
}

bool AProjectileWeaponBase::IsSlideReleased()
{
	return SlideMesh->GetRelativeLocation().X >= -0.5f;;
}

void AProjectileWeaponBase::ApplyRecoil()
{
	if (AVRCharacter* CharacterRefernce = Cast<AVRCharacter>(GetOwner()))
	{
		if (SecondaryHand == EControllerHand::AnyHand)
		{
			CharacterRefernce->ApplyRecoil(PrimaryHand, WeaponMesh->GetForwardVector() * RecoilDistance, RecoilPitch);
		}else
		{
			CharacterRefernce->ApplyRecoil(
				PrimaryHand,
				(WeaponMesh->GetForwardVector() * RecoilDistance) / TwoHandleRecoilDawpeningFactor,
				RecoilPitch / TwoHandleRecoilDawpeningFactor);
			
			CharacterRefernce->ApplyRecoil(
				SecondaryHand,
				(WeaponMesh->GetForwardVector() * RecoilDistance) / TwoHandleRecoilDawpeningFactor,
				RecoilPitch / TwoHandleRecoilDawpeningFactor);
		}
	}
}

void AProjectileWeaponBase::InitMagEjecting()
{
	if (LoadedMagazine)
	{
		MagazineConstraint->SetLinearXLimit(LCM_Free, 0.0f);
		MagazineConstraint->SetLinearYLimit(LCM_Free, 0.0f);
		MagazineConstraint->SetLinearZLimit(LCM_Free, 0.0f);
		
		LoadedMagazine->SetOwner(nullptr);
		
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MagOutSound, MagazineHitBox->GetComponentLocation());
	}
}

void AProjectileWeaponBase::FinishMagEjecting()
{
	MagazineConstraint->BreakConstraint();
	MagazineConstraint->SetLinearXLimit(LCM_Locked, 0.0f);
	MagazineConstraint->SetLinearYLimit(LCM_Locked, 0.0f);
	MagazineConstraint->SetLinearZLimit(LCM_Locked, 0.0f);
	
	LoadedMagazine = nullptr;
	
	GetWorldTimerManager().ClearTimer(MagEjectTimerHandle);
}

void AProjectileWeaponBase::LoadMagzine(AMagazineBase* NewMagazine)
{
	if (!LoadedMagazine && NewMagazine && NewMagazine->IsA(MatchingMagazineClass))
	{
		MagazineConstraint->SetConstrainedComponents(WeaponMesh, NAME_None, NewMagazine->Mesh,NAME_None);
		MagazineConstraint->SetConstraintReferenceFrame(EConstraintFrame::Frame2, MagazineConstraint->GetRelativeTransform());
		
		if (AVRCharacter* MagOwner = Cast<AVRCharacter>(NewMagazine->GetOwner()))
		{
			MagOwner->ReleaseGrabbedActor(NewMagazine);
		}
		
		LoadedMagazine = NewMagazine;
		LoadedMagazine->SetOwner(this);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MagInSound, MagazineHitBox->GetComponentLocation());
	}
}

void AProjectileWeaponBase::OnMagazineBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (AMagazineBase* NewMagazine = Cast<AMagazineBase>(OtherActor))
	{
		LoadMagzine(NewMagazine);
	}
}

UPrimitiveComponent* AProjectileWeaponBase::GetSlideComponent()
{
	return SlideMesh;
}

void AProjectileWeaponBase::FireWeapon()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), TriggerClickSound, TriggerMesh->GetComponentLocation());
	
	switch (SelectedFiringMode)
	{
	case FM_Single:
		{
			Fire();
			break;
		}
	case FM_SemiAuto:
		{
			Fire();
			break;
		}
	case FM_Tripple:
		{
			CurrentFiringCount =0;
			SetActorTickEnabled(true);
			break;
		}
	case FM_FullAuto:
		{
			SetActorTickEnabled(true);
			break;
		}
	}
}

void AProjectileWeaponBase::StopFiringWeapon()
{
	SetActorTickEnabled(
		SelectedFiringMode == FM_Tripple &&
		CurrentFiringCount < MaxFiringCount &&
		!bIsSlideBeingHeld);
}

void AProjectileWeaponBase::ChangeFiringMode()
{
	if (FiringModes.Num() > 1)
	{
		int FoundFiringModeIndex = FiringModes.Find(SelectedFiringMode);
		
		if (FoundFiringModeIndex != -1)
		{
			if (FoundFiringModeIndex >= FiringModes.Num()-1)
			{
				SelectedFiringMode = FiringModes[0];
				SelectorMesh->SetRelativeRotation(FiringModeSelectorRotations[0]);
			}else
			{
				FoundFiringModeIndex++;
				SelectedFiringMode = FiringModes[FoundFiringModeIndex];
				SelectorMesh->SetRelativeRotation(FiringModeSelectorRotations[FoundFiringModeIndex]);
			}
			
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FiringModeSound, SelectorMesh->GetComponentLocation());
		}else
		{
			SelectedFiringMode = FiringModes[0];
			SelectorMesh->SetRelativeRotation(FiringModeSelectorRotations[0]);
		}
	}
}

void AProjectileWeaponBase::ReleaseMag()
{
	InitMagEjecting();
	GetWorldTimerManager().SetTimer(MagEjectTimerHandle, this, &AProjectileWeaponBase::FinishMagEjecting, 0.5f, false);
}

void AProjectileWeaponBase::PullTrigger(float Alpha)
{
	TriggerMesh->SetRelativeRotation(FRotator(FMath::Lerp(0.0f, TriggerMaxRotation, Alpha), 0.0f, 0.0f));
}

void AProjectileWeaponBase::StartMovingSlide(UMotionControllerComponent* MC)
{
	if (!bIsSlideBeingHeld)
	{
		bIsSlideBeingHeld = true;
		SetActorTickEnabled(false);
		RelativeSlideHandStartLocation = UKismetMathLibrary::MakeRelativeTransform(MC->GetComponentTransform(), WeaponMesh->GetComponentTransform()).GetLocation();
	}
}

void AProjectileWeaponBase::StopMovingSlide()
{
	bIsSlideBeingHeld = false;
	
	if (IsSlidePulled())
	{
		if (LoadRound())
		{
			ReleaseSlide();
		}else
		{
			if (bIsSlideStuck)
			{
				ReleaseSlide();
			}else
			{
				bIsSlideStuck = true;
			}
		}
	}else
	{
		if (!IsSlideReleased())
		{
			ReleaseSlide();
		}
	}
}

void AProjectileWeaponBase::MoveSlide(UMotionControllerComponent* MC, FName SlideGripBone)
{
	SlideMesh->SetRelativeLocation(
		FVector(
			FMath::Clamp(
				UKismetMathLibrary::MakeRelativeTransform(MC->GetComponentTransform(), WeaponMesh->GetComponentTransform()).GetLocation().X - RelativeSlideHandStartLocation.X,
				SlideTraveDistance,
				0.0f),
				0.0f,
				0.0f));
	
	if (IsSlidePulled())
	{
		MoveSlideBack();
		EjectShell();
	}else
	{
		if (IsSlideReleased())
		{
			ReleaseSlide();
		}else
		{
			if (bWasSlidePulled && !LoadedAmmo)
			{
				LoadRound();
			}
		}
	}
}

void AProjectileWeaponBase::SetPrimaryHand(EControllerHand Hand)
{
	PrimaryHand = Hand;
}

void AProjectileWeaponBase::SetSecondaryHand(EControllerHand Hand)
{
	SecondaryHand = Hand;
}

UAnimationAsset* AProjectileWeaponBase::GetPrimaryHandTriggerPulledAnimation()
{
	return PrimaryHandTriggerPulledAnimation;
}

UAnimationAsset* AProjectileWeaponBase::GetSlideGripAnimation()
{
	return SlideGripAnimation;
}




