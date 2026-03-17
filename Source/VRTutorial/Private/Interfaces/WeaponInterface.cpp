// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/WeaponInterface.h"

// Add default functionality here for any IWeaponInterface functions that are not pure virtual.
void IWeaponInterface::FireWeapon()
{
}

void IWeaponInterface::StopFiringWeapon()
{
}

void IWeaponInterface::ChangeFiringMode()
{
}

void IWeaponInterface::ReleaseMag()
{
}

void IWeaponInterface::PullTrigger(float ALpha)
{
}

void IWeaponInterface::StartMovingSlide(UMotionControllerComponent* MC)
{
}

void IWeaponInterface::StopMovingSlide()
{
}

void IWeaponInterface::MoveSlide(UMotionControllerComponent* MC, FName SlideGripBone)
{
}

void IWeaponInterface::SetPrimaryHand(EControllerHand Hand)
{
}

void IWeaponInterface::SetSecondaryHand(EControllerHand Hand)
{
}

UPrimitiveComponent* IWeaponInterface::GetSlideComponent()
{
	return nullptr;
}

UAnimationAsset* IWeaponInterface::GetPrimaryHandAnimation()
{
	return nullptr;
}

UAnimationAsset* IWeaponInterface::GetSecondaryHandAnimation()
{
	return nullptr;
}

UAnimationAsset* IWeaponInterface::GetSlideGripAnimation()
{
	return nullptr;
}

UAnimationAsset* IWeaponInterface::GetPrimaryHandTriggerPulledAnimation()
{
	return nullptr;
}
