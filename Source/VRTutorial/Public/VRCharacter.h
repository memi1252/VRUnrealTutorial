// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "VRCharacter.generated.h"



class UVRSaveGame;
class UHandMenuWidget;
class USceneComponent;
class UWidgetComponent;
class UCameraComponent;
class IWeaponInterface;
class USplineComponent;
class USplineMeshComponent;
class UMotionControllerComponent;
class UWidgetInteractionComponent;
class UPhysicalAnimationComponent;
class UPhysicsConstraintComponent;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHandAnimChanged, UAnimationAsset*, HandAnimAseet, UAnimationAsset*, TriggerAnimAsset, bool, bMirrorAnimation);

UENUM(Blueprintable)
enum ETurnStyle
{
	TS_Snap UMETA(DisplayName = "Snap"),
	TS_Smooth UMETA(DIsplayName = "Smooth"),
	TSMAX UMETA(DisplayName = "Max")
};

UENUM(Blueprintable)
enum EMovementDirection
{
	MD_Controller UMETA(DisplayName = "Controller"),
	MD_Camera UMETA(DisplayName = "Camera"),
	MDMAX UMETA(DisplayName = "Max")
};


UCLASS()
class VRTUTORIAL_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()
	
private:
	struct FEnhancedInputActionValueBinding* MoveActionBinding;
	
	FName RightHandBone = "hand_r";
	FName LeftHandBone = "hand_l";
	FName RightAmmoBone = "ammo_bone_r";
	FName LeftAmmoBone = "ammo_bone_l";

public:
#pragma region Variables
	
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* VROrigin;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* RMC;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* LMC;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* RPC;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* LPC;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicalAnimationComponent* PhysicalAnimation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* HandMenu;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetInteractionComponent* WidgetInteraction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* LaserPointerSpline;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineMeshComponent* LaserPointerSplineMesh;
	
#pragma endregion
	
#pragma region Input
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputMappingContext* IMC;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_Move;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_Turn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_Jump;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_Run;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_InteractRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_InteractLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_GrabRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_GrabLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_ReleaseMagRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_ReleaseMagLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_ChangeFiringModeRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_ChangeFiringModeLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_ThumbRightAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_ThumbLeftAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_IndexRightAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_IndexLeftAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_GripRightAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | EnhancedInput")
	UInputAction* IA_GripLeftAxis;
	
#pragma endregion
	
#pragma region Movement
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	float BaseProneSpeed = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	float BaseCrouchSpeed = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	float BaseWalkSpeed = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	float BaseRunSpeed = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	float BaseStepHeight = 45.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	float CrouchRatio = 0.6f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	float ProneRatio = 0.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	bool bIsRunning;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	FVector PreviousCameraLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	FVector CameraVelocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	TEnumAsByte<ETurnStyle> TurnStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Movement")
	TEnumAsByte<EMovementDirection> MovementDirection;

#pragma endregion
	
#pragma region Grabbing
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Grabbing")
	AActor* RightGrabbedActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Grabbing")
	AActor* LeftGrabbedActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Grabbing")
	float GrabRadius = 10.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Grabbing")
	bool bIsRightMainHand;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Grabbing")
	bool bIsLeftMainHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Grabbing")
	TArray<TEnumAsByte<EObjectTypeQuery>> GrabbleObjectTypes = { ObjectTypeQuery7 };
	
#pragma endregion
	
#pragma region Climbing
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	bool bCanClimb = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	bool bIsClimbingRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	bool bIsClimbingLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	float ClimbingbodyZOffset = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	float ClimbingOffsetLerpSpeed = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	TArray<TEnumAsByte<EObjectTypeQuery>> ClimbableObjectTypes = { ObjectTypeQuery1, ObjectTypeQuery2 }; // Static And World Dynamic
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	bool bCanSlideDownOverhang = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	bool bCanLaunchCharacter = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	bool bCanSlipWhileClimbing = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	float CurrentSlippingSpeed = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	FHitResult LatestHit;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	FHitResult PreviousSlidingHit;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	FVector MCStartLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	FVector PreviousMcLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	FVector SlidingStartNormal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	FVector ClimbingOffset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Climbing")
	FQuat LastClimbedActorQuat;
	
#pragma endregion
	
#pragma region Character
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Character")
	float BaseHeight = 191.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Character")
	float BodyZOffset = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Character")
	float BodyCameraOffset = 20.0f;
	
#pragma endregion
	
#pragma region Recoil
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	FVector RightRecoilOffset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	FVector LeftRecoilOffset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	float RightRecoilPitch;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	float LeftRecoilPitch;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	float RecoilLerpTimerInterval = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	float RecoilLerpAplha = 0.2f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	FTimerHandle RightRecoilTH;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Recoil")
	FTimerHandle LeftRecoilTH;
	
#pragma endregion
	
#pragma region FingerValues
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | FingerValues")
	float RightThumbAxisValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | FingerValues")
	float LeftThumbAxisValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | FingerValues")
	float RightIndexAxisValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | FingerValues")
	float LeftIndexAxisValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | FingerValues")
	float RightGripAxisValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | FingerValues")
	float LeftGripAxisValue = 0.0f;
	
#pragma endregion
	
#pragma region Interfaces
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Interfaces")
	TScriptInterface<IWeaponInterface> RightWeaponInterface;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Interfaces")
	TScriptInterface<IWeaponInterface> LeftWeaponInterface;
	
#pragma endregion
	
#pragma region Evenets
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Evenets")
	FHandAnimChanged OnRightHandAnimChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Evenets")
	FHandAnimChanged OnLeftHandAnimChanged;
#pragma endregion
	
#pragma endregion
	
#pragma region Function
	
	// Sets default values for this character's properties
	AVRCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
#pragma region General
	
	static FPhysicalAnimationData GetPhysicalAnimationSettings()
	{
		FPhysicalAnimationData PAD;
		PAD.bIsLocalSimulation = false;
		PAD.OrientationStrength = 5000.0f;
		PAD.AngularVelocityStrength = 500.0f;
		PAD.PositionStrength = 5000.0f;
		PAD.VelocityStrength = 500.0f;
		PAD.MaxLinearForce = 5000.0f;
		PAD.MaxAngularForce = 5000.0f;
		return PAD;
	}
	
	float GetHeightRatio();
	
	float CalculaterFloorLevel(float BodyZOffsetValue = 0.0f, float ClimbingBodyZOffsetValue = 0.0f);
	
	void UpdateVRCollison();
	
	void ApplyHeight();
	
	void CalculateHeight();
	
	void InitPhysicalAnimation();
	
	void EnalbePhysicalAnimation();
	
	UFUNCTION()
	void OnSettingApplied();
	
	void LoadSaveGame();

#pragma endregion
	
#pragma region Movement
	
	void Move(const FInputActionValue& Value);
	
	void StopMoving(const FInputActionValue& Value);
	
	void UpdateMovementSpeed();
	
	void Turn(const FInputActionValue& Value);
	
	void SnapTurn(const FInputActionValue& Value);
	
	void TryJump(const FInputActionValue& Value);
	
	void Run(const FInputActionValue& Value);
	
	void CalculateCameraVelocity();
	
#pragma endregion
	
#pragma region Grabbing
	
	void StartGrabbingRight(const FInputActionValue& Value);
	
	void UpdateGrabbingRight(const FInputActionValue& Value);
	
	void StopGrabbingRight(const FInputActionValue& Value);
	
	void StartGrabbingLeft(const FInputActionValue& Value);
	
	void UpdateGrabbingLeft(const FInputActionValue& Value);
	
	void StopGrabbingLeft(const FInputActionValue& Value);
	
	AActor* StartGrabbing(UPhysicsConstraintComponent* PC, FName BoneToConstrainTo, AActor* OtherGrabbedActor, UMotionControllerComponent* MC, bool& bIsMainHand, bool bIsOtherMainHand);
	
	void UpdateGrabbbing(AActor* GrabbedActor, bool bIsMainHand, UPhysicsConstraintComponent* PC, UMotionControllerComponent* MC, FName BoneToConstrainTo);
	
	void StopGrabbing(UPhysicsConstraintComponent* PC, AActor* GrabbedActor, AActor* OtherGrabbedActor);
	
	void ReleaseGrabbedActor(AActor* GrabbedActor);
	
	FHitResult TraceForObjects(UMotionControllerComponent* MC, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes);
	
#pragma endregion
	
#pragma region Interacting
	
	void InteractRight(const FInputActionValue& Value);
	
	void UpdateInteractRight(const FInputActionValue& Value);
	
	void StopInteractRight(const FInputActionValue& Value);
	
	void InteractLeft(const FInputActionValue& Value);
	
	void UpdateInteractLeft(const FInputActionValue& Value);
	
	void StopInteractLeft(const FInputActionValue& Value);
	
#pragma endregion 
	
#pragma region Weapon Interactions
	
	void ReleaseMagRight(const FInputActionValue& Value);
	
	void ReleaseMagLeft(const FInputActionValue& Value);
	
	void ChangeFiringModeRight(const FInputActionValue& Value);
	
	void ChangeFiringModeLeft(const FInputActionValue& Value);
	
	void ApplyRecoil(EControllerHand Hand, FVector LocationOffset, float Pitch);
	
	UFUNCTION()
	void LerpRecoil(FVector& LocationOffset, float& Pitch, FTimerHandle& RecoilTH);
	
	bool CheckForSlide(AActor* Actor, TScriptInterface<IWeaponInterface>& WI, FName BoneToConstrainTo, UPhysicsConstraintComponent* PC, UMotionControllerComponent* MC);
	
	FName GetGripBoneName(FName HandBoneName);
	
	FName GetHandleBoneName(FName HandBoneName);
	
	bool StartWeaponInteraction(AActor*& GrabbedActor, TScriptInterface<IWeaponInterface>& WI, UMotionControllerComponent* MC, FName BoneToConstrainTo, UPhysicsConstraintComponent* PC);
	
	bool UpdateWeaponInteraction(TScriptInterface<IWeaponInterface> WI, bool bIsMainHand, UMotionControllerComponent* MC, FName BoneToConstrainTo, UPhysicsConstraintComponent* PC);
	
	bool StopWeaponInteraction(TScriptInterface<IWeaponInterface>& WI, bool bIsMainHand, AActor* GrabbedActor, UPhysicsConstraintComponent* PC);
	
#pragma region Ammo Box Interactions
	
	void StartAmmoBoxInteraction(UMotionControllerComponent* MC, AActor*& GrabbedActor, FName BoneToAttachTo);
	
	void StopAmmoBoxInteraction(AActor*& GrabbedActor);
	
#pragma endregion
	
#pragma endregion
	
#pragma region Climbing
	
	void StartClimbing(UMotionControllerComponent* MC, bool& bIsClimbing, bool& bIsOtherClimbing);
	
	void UpdateClimbing(UMotionControllerComponent* MC, bool bIsClimbing, bool bIsOtherClimbing);
	
	void StopClimbing(UMotionControllerComponent* MC, bool& bIsClimbing, bool bIsOtherClimbing);
	
	bool CheckSlipping(UMotionControllerComponent* MC);
	
	bool CheckSecondarySlipping(UMotionControllerComponent* MC);
	
	void SetClimbingOffset(const FHitResult& Hit);
	
	void UpdateClimbingOffSet(const FHitResult& Hit);
	
	FVector UpdateSlipping(const FHitResult& Hit);
	
	void LerpClimbingBodyZOffset(float TargetOffset, float TargetHalfHeight);
	
#pragma endregion
	
#pragma region Finger Values
	
	void ThumbRightAxis(const FInputActionValue& Value);
	
	void ThumbLeftAxis(const FInputActionValue& Value);
	
	void IndexRightAxis(const FInputActionValue& Value);
	
	void IndexLeftAxis(const FInputActionValue& Value);
	
	void GripRightAxis(const FInputActionValue& Value);
	
	void GripLeftAxis(const FInputActionValue& Value);
	
#pragma endregion
	
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
};
