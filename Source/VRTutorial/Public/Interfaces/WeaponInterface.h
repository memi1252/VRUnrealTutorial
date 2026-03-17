// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

class UAnimationAsset;
class UPrimitiveComponent;
class UMotionControllerComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VRTUTORIAL_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void FireWeapon();

	virtual void StopFiringWeapon();

	virtual void ChangeFiringMode();

	virtual void ReleaseMag();

	virtual void PullTrigger(float ALpha);

	virtual void StartMovingSlide(UMotionControllerComponent* MC);

	virtual void StopMovingSlide();

	virtual void MoveSlide(UMotionControllerComponent* MC, FName SlideGripBone);

	virtual void SetPrimaryHand(EControllerHand Hand);

	virtual void SetSecondaryHand(EControllerHand Hand);

	virtual UPrimitiveComponent* GetSlideComponent();

	virtual UAnimationAsset* GetPrimaryHandAnimation();

	virtual UAnimationAsset* GetSecondaryHandAnimation();

	virtual UAnimationAsset* GetSlideGripAnimation();

	virtual UAnimationAsset* GetPrimaryHandTriggerPulledAnimation();
};
