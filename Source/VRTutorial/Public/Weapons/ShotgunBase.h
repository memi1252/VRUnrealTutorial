// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ProjectileWeaponBase.h"
#include "ShotgunBase.generated.h"

class AAmmobase;
class UStaticMeshComponent;
class UAmmoContainerComponent;

UCLASS()
class VRTUTORIAL_API AShotgunBase : public AProjectileWeaponBase
{
	GENERATED_BODY()
	
public:
#pragma region Variables
	
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LatchMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAmmoContainerComponent* AmmoContainer;
	
#pragma endregion
	
#pragma region Latch
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Latch")
	FRotator LatchRotationOffset = FRotator(20.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Latch")
	float LatchResetDelay = 1.0f;
	
#pragma endregion
	
#pragma region Configuration
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Configuration")
	bool bHasInternalAmmoStorage = true;
	
#pragma endregion
	
#pragma endregion
	
#pragma region Functions
	
	AShotgunBase();
	
	virtual void Tick(float DeltaTime) override;
	
	void LoadAmmo(AAmmoBase* AmmoToLoad);
	
	void MoveLatch();
	
	void ResetLatch();
	
	virtual bool LoadRound() override;
	
	virtual void OnMagazineBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) override;
	
#pragma region Interfaces
	
	virtual void StopMovingSlide() override;
	
#pragma endregion
	
#pragma endregion
	
protected:
	virtual void BeginPlay() override;	
};
