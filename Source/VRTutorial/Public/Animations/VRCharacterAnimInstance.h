// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRCharacterAnimInstance.generated.h"

class AVRCharacter;
class UAnimationAsset;

UCLASS()
class VRTUTORIAL_API UVRCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
#pragma region Variables
	
#pragma region General
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | General")
	AVRCharacter* CharacterReference;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | General")
	UAnimationAsset* RightHandAnimAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | General")
	UAnimationAsset* RightHandTriggerAnimAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | General")
	bool bMirrorRightHandAnimation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | General")
	UAnimationAsset* LeftHandAnimAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | General")
	UAnimationAsset* LeftHandTriggerAnimAsset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | General")
	bool bMirrorLeftHandAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | General")
	FVector RightHandOffset = FVector(-2.0f, 0.0f, 9.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | General")
	FVector LeftHandOffset = FVector(2.0f, 0.0f, 9.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | General")
	float CameraOffset = -20.0f;
	
#pragma endregion
	
#pragma region Fingers
	
#pragma region Fingers Collisions
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	float FingerCollisionCheckRadius = 2.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsRightThumbColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsLeftThumbColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsRightIndexColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsLeftIndexColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsRightMiddleColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsLeftMiddleColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsRightRingColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsLeftRingColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsRightPinkyColliding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Collisions")
	bool bIsLeftPinkyColliding;
	
#pragma endregion
	
#pragma region Finger Values
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float FingerValueInterpSpeed = 0.25f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float RightThumbValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float LeftThumbValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float RightIndexValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float LeftIndexValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float RightMiddleValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float LeftMiddleValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float RightRingValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float LeftRingValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float RightPinkyValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Fingers | Values")
	float LeftPinkyValue = 0.0f;
	
#pragma endregion
	
#pragma endregion
	
#pragma endregion
	
#pragma region Functions
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	FTransform GetCameraTransform();
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	FTransform GetRMCTransform();
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	FTransform GetLMCTransform();
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	float GetTotalBodyZOffset();
	
	bool doCollisionCheck(FName BoneName);
	
	void CheckFingerCollisions();
	
	void SetFingerValues();
	
	void CheckIfFingerCanREtract(bool bIsColliding, float& PreviousValue, float newValue);
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	FVector GetCharacterVelocity();
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	FVector GetCharacterPelvisOffset();
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	float GetCharacterPelvisRotation();
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	float GetCharacterHeightRatio();
	
	UFUNCTION(BlueprintCallable, blueprintPure)
	bool GetIsClimbing();
	
	UFUNCTION()
	void OnRightHandAnimChanged(UAnimationAsset* AnimAsset, UAnimationAsset* TriggerAnimAsset, bool bShouldMirrorAnimation);
	
	UFUNCTION()
	void OnLeftHandAnimChanged(UAnimationAsset* AnimAsset, UAnimationAsset* TriggerAnimAsset, bool bShouldMirrorAnimation);
	
#pragma endregion
	
protected:
	
	virtual  void NativeInitializeAnimation() override;
	
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
