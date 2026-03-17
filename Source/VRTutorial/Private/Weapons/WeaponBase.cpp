// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Interfaces/WeaponInterface.h"
#include "Interfaces/DamageInterface.h"

#include <NiagaraSystem.h>
#include <Sound/SoundBase.h>
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/DecalComponent.h>
#include <Materials/MaterialInstance.h>
#include <Components/StaticMeshComponent.h>
#include <PhysicsEngine/PhysicsConstraintComponent.h>

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionProfileName("GrabbableObject");
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetNotifyRigidBodyCollision(true);
	WeaponMesh->SetGenerateOverlapEvents(false);
	
	BladeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BladeMesh"));
	BladeMesh->SetupAttachment(WeaponMesh);
	BladeMesh->SetCollisionProfileName("GrabbableObject");
	BladeMesh->SetNotifyRigidBodyCollision(true);
	BladeMesh->SetGenerateOverlapEvents(false);
	
	PenetrationHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PenetrationHitBox"));
	PenetrationHitBox->SetupAttachment(WeaponMesh);
	WeaponMesh->SetCollisionProfileName("OverlapAll");
	WeaponMesh->SetNotifyRigidBodyCollision(false);
	WeaponMesh->SetGenerateOverlapEvents(true);
	
	PenetrationConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PenetrationConstraint"));
	PenetrationConstraint->SetupAttachment(WeaponMesh);
	PenetrationConstraint->SetLinearXLimit(LCM_Locked, 0.0f);
	PenetrationConstraint->SetLinearYLimit(LCM_Locked, 0.0f);
	PenetrationConstraint->SetLinearZLimit(LCM_Locked, 0.0f);
	PenetrationConstraint->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	PenetrationConstraint->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	PenetrationConstraint->SetAngularTwistLimit(ACM_Locked, 0.0f);
	PenetrationConstraint->SetDisableCollision(true);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponMesh->OnComponentHit.AddDynamic(this, &AWeaponBase::OnBlunHit);

	switch (WeaponMeleeType)
	{
	case WMT_Blunt:
			BladeMesh->DestroyComponent();
			PenetrationHitBox->DestroyComponent();
			PenetrationConstraint->DestroyComponent();
			break;
	case WMT_Sharp:
		BladeMesh->OnComponentHit.AddDynamic(this, &AWeaponBase::OnSharit);
		PenetrationHitBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnPenetrationBeginOverlap);
		PenetrationHitBox->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnPenetrationEndOverlap);
		PenetrationConstraint->SetLinearBreakable(true, LinearBreakable);
		PenetrationConstraint->OnConstraintBroken.AddDynamic(this, &AWeaponBase::OnConstraintBroken);
		break;
	default:
		break;
	}
	
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnConstraintBroken(int BrokenInde)
{
	if (PenetratedComponent)
	{
		PenetrationConstraint->SetConstrainedComponents(BladeMesh, NAME_None, PenetratedComponent,PenetratedBone);
	}
}

void AWeaponBase::DealDamage(UPrimitiveComponent* Component, EDamageType DamageType, FName BoneName)
{
	if (Component && GetWorld()->TimeSince(LastHitTime) >= HitRate)
	{
		LastHitTime = GetWorld()->GetRealTimeSeconds();
		
		if (IDamageInterface* DI = Cast<IDamageInterface>(Component->GetWorld()))
		{
			DI->Damage(DamageType, Damage, BoneName);
		}
	}
}

void AWeaponBase::SpawnEffect(USoundBase* SFX, UNiagaraSystem* VFX, UMaterialInstance* Decal, UPrimitiveComponent* Comp,
	FName BoneName, FVector Location, FVector ImpactNormal)
{
	if (GetWorld()->TimeSince(LastHitTime) >= HitRate)
	{
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(ImpactNormal);
		NewRotation.Roll = -WeaponMesh->GetComponentRotation().Roll;
		
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX, Location);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFX, Location, NewRotation);
		
		if (UDecalComponent* SpawnedDecal = UGameplayStatics::SpawnDecalAttached(Decal, FVector(3.0f), Comp, BoneName, Location, NewRotation,
			EAttachLocation::KeepWorldPosition, DecalLifetime))
		{
			SpawnedDecal->SetFadeScreenSize(DecalFadeScreenSize);
		}
	}
}

void AWeaponBase::OnBlunHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetOwner() && !PenetratedComponent && WeaponMesh->GetPhysicsLinearVelocity().Size() > MinDamageSpeed)
	{
		SpawnEffect(HitSound, HitEffect, HitDecal, OtherComp, Hit.BoneName, Hit.Location, Hit.ImpactNormal);
		DealDamage(OtherComp, DT_Hit, Hit.BoneName);
	}
}

void AWeaponBase::OnSharit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OverlappedPenetrableActor && WeaponMesh->GetPhysicsLinearVelocity().Size() > MinDamageSpeed)
	{
		PenetratedComponent = OtherComp;
		PenetratedBone = Hit.BoneName;
		
		PenetrationConstraint->SetConstrainedComponents(BladeMesh, NAME_None, PenetratedComponent, PenetratedBone);
		
		SpawnEffect(PenetrationSound, PenetrationEffect, PenetrationDecal, OtherComp, Hit.BoneName, Hit.Location, Hit.ImpactNormal);
		DealDamage(OtherComp, DT_Penetration, Hit.BoneName);
	}
}

void AWeaponBase::OnPenetrationBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (GetWorld() && OtherActor->ActorHasTag("penetrable") && OtherActor != GetOwner())
	{
		OverlappedPenetrableActor = OtherActor;	
	}
}

void AWeaponBase::OnPenetrationEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (OtherActor == OverlappedPenetrableActor)
	{
		OverlappedPenetrableActor = nullptr;
		PenetratedComponent = nullptr;
		PenetratedBone = NAME_None;
		PenetrationConstraint->BreakConstraint();
	}
}

UAnimationAsset* AWeaponBase::GetPrimaryHandAnimation()
{
	return PrimaryHandAnimation;
}

UAnimationAsset* AWeaponBase::GetSecondaryHandAnimation()
{
	return SecondaryHandAnimation;
}

