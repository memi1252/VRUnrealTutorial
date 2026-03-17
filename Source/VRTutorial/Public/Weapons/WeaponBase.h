// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enums/WeaponEnumTypes.h"
#include "../Interfaces/WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UMaterialInstance;
class UStaticMeshComponent;
class UPhysicalConstraintComponent;

UCLASS()
class VRTUTORIAL_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
#pragma region Variables
	
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BladeMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PenetrationConstraint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* PenetrationHitBox;
	
#pragma endregion
	
#pragma region Melee
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	float LinearBreakable = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	float HitRate = 0.2f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	float LastHitTime = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	float MinDamageSpeed = 200.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	FName PenetratedBone;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	AActor* OverlappedPenetrableActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	UPrimitiveComponent* PenetratedComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Melee")
	TEnumAsByte<EWeaponMeleeType>WeaponMeleeType;
	
#pragma endregion
	
#pragma region SFX
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | SFX")
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | SFX")
	USoundBase* PenetrationSound;
	
#pragma endregion
	
#pragma region VFX
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | VFX")
	UNiagaraSystem* HitEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | VFX")
	UNiagaraSystem* PenetrationEffect;
	
#pragma endregion
	
#pragma region Decals
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Decals")
	UMaterialInstance* HitDecal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Decals")
	UMaterialInstance* PenetrationDecal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Decals")
	float DecalLifetime = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Decals")
	float DecalFadeScreenSize = 0.001f;
	
#pragma endregion
	
#pragma region Animations;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Animations")
	UAnimationAsset* PrimaryHandAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Animations")
	UAnimationAsset* SecondaryHandAnimation;
	
#pragma endregion
	
#pragma endregion
	
#pragma region function
	// Sets default values for this actor's properties
	AWeaponBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnConstraintBroken(int BrokenInde);
	
	void DealDamage(UPrimitiveComponent* Component, EDamageType DamageType, FName BoneName);
	
	void SpawnEffect(USoundBase* SFX, UNiagaraSystem* VFX, UMaterialInstance* Decal, UPrimitiveComponent* Comp, FName BoneName = NAME_None,
		FVector Location = FVector::ZeroVector, FVector ImpactNormal = FVector::ZeroVector);
	
	UFUNCTION()
	void OnBlunHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnSharit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnPenetrationBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnPenetrationEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);
	
	virtual UAnimationAsset* GetPrimaryHandAnimation();
	
	virtual UAnimationAsset* GetSecondaryHandAnimation();
	
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
