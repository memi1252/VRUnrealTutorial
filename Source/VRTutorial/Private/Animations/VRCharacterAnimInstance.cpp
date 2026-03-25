// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/VRCharacterAnimInstance.h"
#include "VRCharacter.h"

#include <Camera/CameraComponent.h>
#include <Animation/AnimationAsset.h>
#include <Kismet/KismetMathLibrary.h>
#include <MotionControllerComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Components/CapsuleComponent.h>

void UVRCharacterAnimInstance::NativeInitializeAnimation()
{
	//Super::NativeInitializeAnimation();
}

void UVRCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (AVRCharacter* Character = Cast<AVRCharacter>(TryGetPawnOwner()))
	{
		CharacterReference = Character;
		CharacterReference->OnRightHandAnimChanged.AddDynamic(this, &UVRCharacterAnimInstance::OnRightHandAnimChanged);
		CharacterReference->OnLeftHandAnimChanged.AddDynamic(this, &UVRCharacterAnimInstance::OnLeftHandAnimChanged);
	}
}

void UVRCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (CharacterReference)
	{
		CheckFingerCollisions();
		SetFingerValues();
	}
}

FTransform UVRCharacterAnimInstance::GetCameraTransform()
{
	if (CharacterReference)
	{
		FTransform CameraTransform;
		
		CameraTransform.SetLocation(CharacterReference->Camera->GetComponentLocation() + CharacterReference->Camera->GetForwardVector() * CameraOffset);
		CameraTransform.SetRotation(CharacterReference->Camera->GetComponentQuat());
		CameraTransform.SetScale3D(CharacterReference->Camera->GetComponentScale());
		return CameraTransform;
	}
	return FTransform();
}

FTransform UVRCharacterAnimInstance::GetRMCTransform()
{
	if (CharacterReference)
	{
		FTransform RMCTransform;
		
		RMCTransform.SetLocation(CharacterReference->RMC->GetComponentLocation()+
			CharacterReference->RMC->GetForwardVector() * RightHandOffset.X +
			CharacterReference->RMC->GetRightVector() * RightHandOffset.Y +
			CharacterReference->RMC->GetUpVector() * RightHandOffset.Z+
			CharacterReference->RightRecoilOffset);
		
		RMCTransform.SetRotation(CharacterReference->RMC->GetComponentQuat());
		RMCTransform.SetScale3D(CharacterReference->RMC->GetComponentScale());
		
		return RMCTransform;
	}
	return FTransform();
}

FTransform UVRCharacterAnimInstance::GetLMCTransform()
{
	if (CharacterReference)
	{
		FTransform LMCTransform;
		
		LMCTransform.SetLocation(CharacterReference->LMC->GetComponentLocation()+
			CharacterReference->LMC->GetForwardVector() * LeftHandOffset.X +
			CharacterReference->LMC->GetRightVector() * LeftHandOffset.Y +
			CharacterReference->LMC->GetUpVector() * LeftHandOffset.Z+
			CharacterReference->LeftRecoilOffset);
		
		LMCTransform.SetRotation(CharacterReference->LMC->GetComponentQuat());
		LMCTransform.SetScale3D(CharacterReference->LMC->GetComponentScale());
		
		return LMCTransform;
	}
	return FTransform();
}

float UVRCharacterAnimInstance::GetTotalBodyZOffset()
{
	if (CharacterReference)
	{
		return CharacterReference->BodyZOffset + CharacterReference->ClimbingbodyZOffset;
	}
	return 0.0f;
}

bool UVRCharacterAnimInstance::doCollisionCheck(FName BoneName)
{
	FHitResult Hit;
	return UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		CharacterReference->GetMesh()->GetSocketLocation(BoneName),
		CharacterReference->GetMesh()->GetSocketLocation(BoneName),
		FingerCollisionCheckRadius,
		TraceTypeQuery1,
		false,
		{},
		EDrawDebugTrace::None,
		Hit,
		true);
}

void UVRCharacterAnimInstance::CheckFingerCollisions()
{
	if (!RightHandAnimAsset)
	{
		bIsRightThumbColliding = doCollisionCheck("thumb_03_r");
		bIsRightIndexColliding = doCollisionCheck("index_03_r");
		bIsRightMiddleColliding = doCollisionCheck("middle_03_r");
		bIsRightRingColliding = doCollisionCheck("ring_03_r");
		bIsRightPinkyColliding = doCollisionCheck("pinky_03_r");
	}
	
	if (!LeftHandAnimAsset)
	{
		bIsLeftThumbColliding = doCollisionCheck("thumb_03_l");
		bIsLeftIndexColliding = doCollisionCheck("index_03_l");
		bIsLeftMiddleColliding = doCollisionCheck("middle_03_l");
		bIsLeftRingColliding = doCollisionCheck("ring_03_l");
		bIsLeftPinkyColliding = doCollisionCheck("pinky_03_l");
	}
}

void UVRCharacterAnimInstance::SetFingerValues()
{
	CheckIfFingerCanREtract(bIsRightThumbColliding, RightThumbValue, CharacterReference->RightThumbAxisValue);
	CheckIfFingerCanREtract(bIsLeftThumbColliding, LeftThumbValue, CharacterReference->LeftThumbAxisValue);
	
	CheckIfFingerCanREtract(bIsRightIndexColliding, RightIndexValue, CharacterReference->RightIndexAxisValue);
	CheckIfFingerCanREtract(bIsLeftIndexColliding, LeftIndexValue, CharacterReference->LeftIndexAxisValue);
	
	CheckIfFingerCanREtract(bIsRightMiddleColliding, RightMiddleValue, CharacterReference->RightGripAxisValue);
	CheckIfFingerCanREtract(bIsLeftMiddleColliding, LeftMiddleValue, CharacterReference->LeftGripAxisValue);
	
	CheckIfFingerCanREtract(bIsRightRingColliding, RightRingValue, CharacterReference->RightGripAxisValue);
	CheckIfFingerCanREtract(bIsLeftRingColliding, LeftRingValue, CharacterReference->LeftGripAxisValue);
	
	CheckIfFingerCanREtract(bIsRightPinkyColliding, RightPinkyValue, CharacterReference->RightGripAxisValue);
	CheckIfFingerCanREtract(bIsLeftPinkyColliding, LeftPinkyValue, CharacterReference->LeftGripAxisValue);
}

void UVRCharacterAnimInstance::CheckIfFingerCanREtract(bool bIsColliding, float& PreviousValue, float newValue)
{
	if (bIsColliding)
	{
		if (newValue > PreviousValue)
		{
			return;
		}
	}
	
	PreviousValue = FMath::Lerp(PreviousValue, newValue, FingerValueInterpSpeed);
}

FVector UVRCharacterAnimInstance::GetCharacterVelocity()
{
	if (CharacterReference)
	{
		return CharacterReference->CameraVelocity;
	}
	return FVector::ZeroVector;
}

FVector UVRCharacterAnimInstance::GetCharacterPelvisOffset()
{
	if (CharacterReference)
	{
		float HeightRatio = CharacterReference->GetHeightRatio();
		
		float HalfHeight = UKismetMathLibrary::MapRangeClamped(
			HeightRatio,
			CharacterReference->ProneRatio,
			1.0f,
			10.0f,
			CharacterReference->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		
		if (HalfHeight * 2.0f > CharacterReference->BaseHeight)
		{
			HalfHeight = CharacterReference->BaseHeight / 2.0f;
		}
		
		float YOffset = UKismetMathLibrary::MapRangeClamped(
			HeightRatio,
			CharacterReference->ProneRatio,
			1.0f,
			-CharacterReference->BaseHeight / 3.0f,
			0.0f);
		
		float ZOffset = HalfHeight + ((CharacterReference->ClimbingbodyZOffset / 2.0f) * UKismetMathLibrary::MapRangeClamped(HeightRatio, CharacterReference->ProneRatio, 1.0f, 0.0f, 1.0f));
		return FVector(0.0f, YOffset, ZOffset);
	}
	return FVector::ZeroVector;
}

float UVRCharacterAnimInstance::GetCharacterPelvisRotation()
{
	if (CharacterReference)
	{
		return UKismetMathLibrary::MapRangeClamped(
			CharacterReference->GetHeightRatio(),
			CharacterReference->ProneRatio,
			CharacterReference->CrouchRatio,
			90.0f,
			0.0f);
	}
	return 0.0f;
}

float UVRCharacterAnimInstance::GetCharacterHeightRatio()
{
	if (CharacterReference)
	{
		return CharacterReference->GetHeightRatio();
	}
	
	return 0.0f;
}

bool UVRCharacterAnimInstance::GetIsClimbing()
{
	if (CharacterReference)
	{
		return CharacterReference->bCanClimb && (CharacterReference->bIsClimbingRight || CharacterReference->bIsClimbingLeft);
	}
	return false;
}

void UVRCharacterAnimInstance::OnRightHandAnimChanged(UAnimationAsset* AnimAsset, UAnimationAsset* TriggerAnimAsset,
	bool bShouldMirrorAnimation)
{
	RightHandAnimAsset = AnimAsset;
	RightHandTriggerAnimAsset = TriggerAnimAsset;
	bMirrorRightHandAnimation = bShouldMirrorAnimation;
	
	if (RightHandAnimAsset)
	{
		bIsRightThumbColliding = false;
		bIsRightIndexColliding = false;
		bIsRightMiddleColliding = false;
		bIsRightRingColliding = false;
		bIsRightPinkyColliding = false;
	}
}

void UVRCharacterAnimInstance::OnLeftHandAnimChanged(UAnimationAsset* AnimAsset, UAnimationAsset* TriggerAnimAsset,
	bool bShouldMirrorAnimation)
{
	LeftHandAnimAsset = AnimAsset;
	LeftHandTriggerAnimAsset = TriggerAnimAsset;
	bMirrorLeftHandAnimation = bShouldMirrorAnimation;
	
	if (LeftHandAnimAsset)
	{
		bIsLeftThumbColliding = false;
		bIsLeftIndexColliding = false;
		bIsLeftMiddleColliding = false;
		bIsLeftRingColliding = false;
		bIsLeftPinkyColliding = false;
	}
}
