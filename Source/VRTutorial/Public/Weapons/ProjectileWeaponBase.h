// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "../Enums/WeaponEnumTypes.h"
#include "ProjectileWeaponBase.generated.h"

class AAmmoBase;
class USoundBase;
class UBoxComponent;
class AMagazineBase;
class UNiagaraSystem;
class UStaticMeshComponent;
class UMotionControllerComponent;
class UPhysicsConstraintComponent;

UCLASS()
class VRTUTORIAL_API AProjectileWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
private:
	FTimerHandle MagEjectTimerHandle;
	
public:
	
#pragma region Variables
	
#pragma region Component
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SlideMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TriggerMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SelectorMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* MagazineConstraint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MagazineHitBox;
	
#pragma endregion 
	
#pragma region Firing
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Firing")
	float FireRate = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Firing")
	float LastFireTime = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Firing")
	int ProjectileCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Firing")
	float MaxProjectileSpread;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Firing")
	float TriggerMaxRotation = -20.0f;
	
#pragma endregion
	
#pragma region Recoil
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	float RecoilDistance = -5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	float RecoilPitch = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	float TwoHandleRecoilDawpeningFactor = 2.0f;
	
#pragma endregion
	
#pragma region Firing Modes
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Firing Modes")
	TArray<TEnumAsByte<EFiringMode>> FiringModes = {FM_SemiAuto};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Firing Modes")
	TEnumAsByte<EFiringMode> SelectedFiringMode = FM_SemiAuto;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Firing Modes")
	TArray<FRotator> FiringModeSelectorRotations = { FRotator::ZeroRotator};
	
#pragma endregion
	
#pragma region Tripple Fire
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Tripple Fire")
	int CurrentFiringCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Tripple Fire")
	int MaxFiringCount = 3;
	
#pragma endregion
	
#pragma region Magazine
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Magazine")
	TSubclassOf<AMagazineBase> MatchingMagazineClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Magazine")
	AMagazineBase* LoadedMagazine;
	
#pragma endregion
	
#pragma region Ammo
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Ammo")
	AAmmoBase* LoadedAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Ammo")
	FVector ShellEjectTrajectory = FVector(0.0f, 1.0f, 1.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Ammo")
	float ShellEjectForce = 200.0f;
	
#pragma endregion
	
#pragma region Slide
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Slide")
	float SlideTraveDistance = -5.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Slide")
	bool bWasSlidePulled;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Slide")
	bool bWasSlideReleased;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Slide")
	bool bIsSlideBeingHeld;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Slide")
	bool bIsSlideStuck;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Slide")
	FVector RelativeSlideHandStartLocation;
	
#pragma endregion
	
#pragma region Hands
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Hands")
	EControllerHand PrimaryHand = EControllerHand::AnyHand;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Varialbes | Hands")
	EControllerHand SecondaryHand = EControllerHand::AnyHand;
	
#pragma endregion
	
#pragma region Effects
	
#pragma region VFX
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Effects | VFX")
	UNiagaraSystem* MuzzleFlash;
	
#pragma endregion
	
#pragma region SFX
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Effects | SFX")
	USoundBase* MagInSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Effects | SFX")
	USoundBase* MagOutSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Effects | SFX")
	USoundBase* FiringSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Effects | SFX")
	USoundBase* TriggerClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Effects | SFX")
	USoundBase* SlideBackSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Effects | SFX")
	USoundBase* SlideReleaseSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Effects | SFX")
	USoundBase* FiringModeSound;

#pragma endregion
	
#pragma endregion
	
#pragma region Names
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Names")
	FName LoadedAmmoBone = "Loaded_ammo_bone";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Names")
	FName MuzzleBone = "muzzle_bone";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Names")
	FName MagazineBone = "magazine_bone";

#pragma endregion
	
#pragma region Animations
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Animations")
	UAnimationAsset* PrimaryHandTriggerPulledAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Animations")
	UAnimationAsset* SlideGripAnimation;
	
#pragma endregion
	
#pragma endregion
	
#pragma region Functions
	
	AProjectileWeaponBase();
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool Fire();
	
	void EjectShell();
	
	UFUNCTION(BlueprintCallable)
	void MoveSlideBack();
	
	UFUNCTION(BlueprintCallable)
	void ReleaseSlide();
	
	UFUNCTION(BlueprintCallable)
	EMagCheckReturnType CheckMag();
	
	UFUNCTION(BlueprintCallable)
	EAmmoCheckReturnType CheckAmmo();
	
	bool SpawnLoadedAmmo(TSubclassOf<AAmmoBase> AmmoClass);
	
	virtual bool LoadRound();
	
	bool IsSlidePulled();
	
	bool IsSlideReleased();
	
	void ApplyRecoil();
	
	void InitMagEjecting();
	
	void FinishMagEjecting();
	
	void LoadMagzine(AMagazineBase* NewMagazine);
	
	UFUNCTION()
	virtual void OnMagazineBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	
#pragma region Weapon Interface

	virtual UPrimitiveComponent* GetSlideComponent();
	
	virtual void FireWeapon();
	
	virtual void StopFiringWeapon();
	
	virtual void ChangeFiringMode();
	
	virtual void ReleaseMag();
	
	virtual void PullTrigger(float Alpha);
	
	virtual void StartMovingSlide(UMotionControllerComponent* MC);
	
	virtual void StopMovingSlide();
	
	virtual void MoveSlide(UMotionControllerComponent* MC, FName SlideGripBone);
	
	virtual void SetPrimaryHand(EControllerHand Hand);
	
	virtual void SetSecondaryHand(EControllerHand Hand);
	
	virtual UAnimationAsset* GetPrimaryHandTriggerPulledAnimation();
	
	virtual UAnimationAsset* GetSlideGripAnimation();
	
#pragma endregion

#pragma endregion
	
protected:
	virtual void BeginPlay() override;
};
