// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "VRSaveGame.h"
#include "Weapons/WeaponBase.h"
#include "Weapons/ShotgunBase.h"
#include "widgets/HandMenuWidget.h"
#include "widgets/SettingsMenuWidget.h"
#include "Interfaces/WeaponInterface.h"
#include "Weapons/Magazines/AmmoBoxBase.h"
#include "Weapons/Magazines/Ammo/AmmoBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include <EnhancedInputLibrary.h>
#include <KismetAnimationLibrary.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMaterialLibrary.h>
#include <Components/SceneComponent.h>
#include <MotionControllerComponent.h>
#include <Components/WidgetComponent.h>
#include <Components/SplineComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SplineMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <PhysicalMaterials/PhysicalMaterial.h>
#include <Components/WidgetInteractionComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <PhysicsEngine/PhysicalAnimationComponent.h>
#include <PhysicsEngine/PhysicsConstraintComponent.h>

#include "Iris/Core/IrisDebugging.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	GetCharacterMovement()->MaxStepHeight = BaseStepHeight;
	
	GetCapsuleComponent()->SetCapsuleSize(22.0f, 88.0f);
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));;
	VROrigin->SetupAttachment(GetCapsuleComponent());
	VROrigin->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);
	
	RMC = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RMC"));
	RMC->SetupAttachment(VROrigin);
	RMC->SetTrackingSource(EControllerHand::Right);
	
	LMC = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LMC"));
	LMC->SetupAttachment(VROrigin);
	LMC->SetTrackingSource(EControllerHand::Left);
	
	RPC = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RPC"));
	RPC->SetupAttachment(GetMesh(), RightHandBone);
	RPC->SetRelativeLocation(FVector(-7.0f, 3.0f, 0.0f));
	RPC->SetRelativeRotation(FRotator(0.0f, 100.0f, 0.0f));
	RPC->SetLinearXLimit(LCM_Locked, 0.0f);
	RPC->SetLinearYLimit(LCM_Locked, 0.0f);
	RPC->SetLinearZLimit(LCM_Locked, 0.0f);
	RPC->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	RPC->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	RPC->SetAngularTwistLimit(ACM_Locked, 0.0f);
	RPC->SetDisableCollision(true);
	
	LPC = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LPC"));
	LPC->SetupAttachment(GetMesh(), LeftHandBone);
	LPC->SetRelativeLocation(FVector(7.0f, -3.0f, 0.0f));
	LPC->SetRelativeRotation(FRotator(0.0f, 0.0f, 100.0f));
	LPC->SetLinearXLimit(LCM_Locked, 0.0f);
	LPC->SetLinearYLimit(LCM_Locked, 0.0f);
	LPC->SetLinearZLimit(LCM_Locked, 0.0f);
	LPC->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	LPC->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	LPC->SetAngularTwistLimit(ACM_Locked, 0.0f);
	LPC->SetDisableCollision(true);
	
	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimation"));
	
	HandMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("HandMenu"));
	HandMenu->SetupAttachment(LMC);
	HandMenu->AddRelativeRotation(FRotator(0.0f, -90.0f, -110.0f));
	HandMenu->SetDrawSize(FVector2D(1920.0f, 1080.0f));
	HandMenu->SetWorldScale3D(FVector(0.05f));
	HandMenu->SetCollisionProfileName("Menu");
	
	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->SetupAttachment(RMC);
	WidgetInteraction->AddLocalRotation(FRotator(-70.0f, 0.0f, 0.0f));
	WidgetInteraction->TraceChannel = ECC_GameTraceChannel3;
	
	LaserPointerSpline = CreateDefaultSubobject<USplineComponent>(TEXT("LaserPointerSline"));
	LaserPointerSpline->SetupAttachment(RMC);
	
	LaserPointerSplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("LaserPointerSplineMesh"));
	LaserPointerSplineMesh->SetupAttachment(LaserPointerSpline);
	LaserPointerSplineMesh->SetCastShadow(false);
	LaserPointerSplineMesh->SetMobility(EComponentMobility::Movable);
	LaserPointerSplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	LoadSaveGame();
	
	ApplyHeight();
	InitPhysicalAnimation();
	
	if (UHandMenuWidget* HandMenuWidget = Cast<UHandMenuWidget>(HandMenu->GetWidget()))
	{
		if (HandMenuWidget->SettingsMenuWidget)
		{
			HandMenuWidget->SettingsMenuWidget->OnSettingsApplied.AddDynamic(this, &AVRCharacter::OnSettingApplied);
		}
	}
}

void AVRCharacter::PossessedBy(AController* NewController)
{
	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
	}
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateVRCollison();
	CalculateCameraVelocity();
	
	if (LMC->GetRightVector().Equals(Camera->GetForwardVector(), 0.5f))
	{
		if (!HandMenu->IsVisible())
		{
			HandMenu->SetVisibility(true);
		}
	}
	else
	{
		if (HandMenu->IsVisible())
		{
			HandMenu->SetVisibility(false);
		}
	}
	
	if (WidgetInteraction->GetHoveredWidgetComponent())
	{
		LaserPointerSpline->SetLocationAtSplinePoint(0, WidgetInteraction->GetComponentLocation(), ESplineCoordinateSpace::World);
		LaserPointerSpline->SetLocationAtSplinePoint(1, WidgetInteraction->GetLastHitResult().Location, ESplineCoordinateSpace::World);
	}else
	{
		FVector StartPos = LaserPointerSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		LaserPointerSpline->SetLocationAtSplinePoint(1, StartPos, ESplineCoordinateSpace::World);
	}
	
	FVector StartPos;
	FVector StartTangent;
	FVector EndPos;
	FVector EndTangent;
	
	LaserPointerSpline->GetLocationAndTangentAtSplinePoint(0, StartPos, StartTangent, ESplineCoordinateSpace::World);
	LaserPointerSpline->GetLocationAndTangentAtSplinePoint(1, EndPos, EndTangent, ESplineCoordinateSpace::World);
	
	LaserPointerSplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos / LaserPointerSplineMesh->GetComponentScale().X, EndTangent);

}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* ETC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//moving
		ETC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AVRCharacter::Move);
		ETC->BindAction(IA_Move, ETriggerEvent::Completed, this, &AVRCharacter::StopMoving);
		ETC->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AVRCharacter::Turn);
		ETC->BindAction(IA_Turn, ETriggerEvent::Completed, this, &AVRCharacter::SnapTurn);
		ETC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AVRCharacter::TryJump);
		ETC->BindAction(IA_Run, ETriggerEvent::Started, this, &AVRCharacter::Run);
		
		//Grabbing
		ETC->BindAction(IA_GrabRight, ETriggerEvent::Started, this, &AVRCharacter::StartGrabbingRight);
		ETC->BindAction(IA_GrabRight, ETriggerEvent::Triggered, this, &AVRCharacter::UpdateGrabbingRight);
		ETC->BindAction(IA_GrabRight, ETriggerEvent::Completed, this, &AVRCharacter::StopGrabbingRight);
		ETC->BindAction(IA_GrabLeft, ETriggerEvent::Started, this, &AVRCharacter::StartGrabbingLeft);
		ETC->BindAction(IA_GrabLeft, ETriggerEvent::Triggered, this, &AVRCharacter::UpdateGrabbingLeft);
		ETC->BindAction(IA_GrabLeft, ETriggerEvent::Completed, this, &AVRCharacter::StopGrabbingLeft);
		
		//Interacting
		ETC->BindAction(IA_InteractRight, ETriggerEvent::Started, this, &AVRCharacter::InteractRight);
		ETC->BindAction(IA_InteractRight, ETriggerEvent::Triggered, this, &AVRCharacter::UpdateInteractRight);
		ETC->BindAction(IA_InteractRight, ETriggerEvent::Completed, this, &AVRCharacter::StopInteractRight);
		ETC->BindAction(IA_InteractLeft, ETriggerEvent::Started, this, &AVRCharacter::InteractLeft);
		ETC->BindAction(IA_InteractLeft, ETriggerEvent::Triggered, this, &AVRCharacter::UpdateInteractLeft);
		ETC->BindAction(IA_InteractLeft, ETriggerEvent::Completed, this, &AVRCharacter::StopInteractLeft);
		
		//Weapon Interactions
		ETC->BindAction(IA_ReleaseMagRight, ETriggerEvent::Started, this, &AVRCharacter::ReleaseMagRight);
		ETC->BindAction(IA_ReleaseMagLeft, ETriggerEvent::Started, this, &AVRCharacter::ReleaseMagLeft);
		ETC->BindAction(IA_ChangeFiringModeRight, ETriggerEvent::Started, this, &AVRCharacter::ChangeFiringModeRight);
		ETC->BindAction(IA_ChangeFiringModeLeft, ETriggerEvent::Started, this, &AVRCharacter::ChangeFiringModeLeft);
		
		//Finger Values
		//Thumb
		ETC->BindAction(IA_ThumbRightAxis, ETriggerEvent::Triggered, this, &AVRCharacter::ThumbRightAxis);
		ETC->BindAction(IA_ThumbRightAxis, ETriggerEvent::Completed, this, &AVRCharacter::ThumbRightAxis);
		ETC->BindAction(IA_ThumbLeftAxis, ETriggerEvent::Triggered, this, &AVRCharacter::ThumbLeftAxis);
		ETC->BindAction(IA_ThumbLeftAxis, ETriggerEvent::Completed, this, &AVRCharacter::ThumbLeftAxis);
		
		//Index
		ETC->BindAction(IA_IndexRightAxis, ETriggerEvent::Triggered, this, &AVRCharacter::IndexRightAxis);
		ETC->BindAction(IA_IndexRightAxis, ETriggerEvent::Completed, this, &AVRCharacter::IndexRightAxis);
		ETC->BindAction(IA_IndexLeftAxis, ETriggerEvent::Triggered, this, &AVRCharacter::IndexLeftAxis);
		ETC->BindAction(IA_IndexLeftAxis, ETriggerEvent::Completed, this, &AVRCharacter::IndexLeftAxis);
		
		//Grip
		ETC->BindAction(IA_GripRightAxis, ETriggerEvent::Triggered, this, &AVRCharacter::GripRightAxis);
		ETC->BindAction(IA_GripRightAxis, ETriggerEvent::Completed, this, &AVRCharacter::GripRightAxis);
		ETC->BindAction(IA_GripLeftAxis, ETriggerEvent::Triggered, this, &AVRCharacter::GripLeftAxis);
		ETC->BindAction(IA_GripLeftAxis, ETriggerEvent::Completed, this, &AVRCharacter::GripLeftAxis);
		
		MoveActionBinding = &ETC->BindActionValue(IA_Move);
	}
}

float AVRCharacter::GetHeightRatio()
{
	FHitResult Hit;
	
	if (GetWorld()->LineTraceSingleByChannel(
		Hit,
		Camera->GetComponentLocation() + FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleRadius()),
		Camera->GetComponentLocation() - FVector(0.0f, 0.0f, BaseHeight),
		ECC_Visibility))
	{
		return FMath::Clamp(Hit.Distance / BaseHeight, 0.0f, 1.0f);
	}
	return 1.0f;
}

float AVRCharacter::CalculaterFloorLevel(float BodyZOffsetValue, float ClimbingBodyZOffsetValue)
{
	return (GetCapsuleComponent()->GetComponentLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) + BodyZOffsetValue + ClimbingBodyZOffsetValue;
}

void AVRCharacter::UpdateVRCollison()
{
	FVector HMDOffset = Camera->GetComponentLocation() - GetCapsuleComponent()->GetComponentLocation();
	HMDOffset.Z = 0.0f;
	GetCapsuleComponent()->AddWorldOffset(HMDOffset, true);
	VROrigin->AddWorldOffset(-HMDOffset);
	VROrigin->SetWorldLocation(FVector(
		VROrigin->GetComponentLocation().X,
		VROrigin->GetComponentLocation().Y,
		CalculaterFloorLevel(BodyZOffset, ClimbingOffset.Z)));
	
	HMDOffset.Z = Camera->GetComponentLocation().Z - GetCapsuleComponent()->GetComponentLocation().Z;
	
	if (HMDOffset.Z < 0.0f)
	{
		VROrigin->AddWorldOffset(FVector(0.0f, 0.0f, -HMDOffset.Z));
	}
	
	float HalfHight = ((Camera->GetComponentLocation().Z + GetCapsuleComponent()->GetScaledCapsuleRadius()) - VROrigin->GetComponentLocation().Z) / 2.0f;
	
	if (bCanClimb && (bIsClimbingRight || bIsClimbingLeft))
	{
		float TargetOffset = -(BaseHeight - GetCapsuleComponent()->GetScaledCapsuleRadius() + 2.0f);
		
		if (ClimbingbodyZOffset > TargetOffset)
		{
			LerpClimbingBodyZOffset(TargetOffset, GetCapsuleComponent()->GetScaledCapsuleRadius());
		}
	}
	else
	{
		if (!FMath::IsNearlyZero(ClimbingbodyZOffset, 0.1f))
		{
			LerpClimbingBodyZOffset(0.0f, HalfHight);
		}else
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(HalfHight);
		}
	}
	
	FHitResult Hit;
	
	if (UKismetSystemLibrary::LineTraceSingleByProfile(
		GetWorld(),
		GetCapsuleComponent()->GetComponentLocation(),
		FVector(
			GetCapsuleComponent()->GetComponentLocation().X,
			GetCapsuleComponent()->GetComponentLocation().Y,
			Camera->GetComponentLocation().Z + GetCapsuleComponent()->GetScaledCapsuleRadius()),
			GetCapsuleComponent()->GetCollisionProfileName(),
			false,
			{ this },
			EDrawDebugTrace::None,
			Hit,
			true))
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(Hit.Distance - 0.1f);
		
		VROrigin->AddWorldOffset(FVector(0.0f, 0.0f, Hit.Location.Z - (Camera->GetComponentLocation().Z + GetCapsuleComponent()->GetScaledCapsuleRadius())));
	}
	
	if (UKismetSystemLibrary::LineTraceSingleByProfile(
		GetWorld(),
		GetCapsuleComponent()->GetComponentLocation(),
		FVector(
			GetCapsuleComponent()->GetComponentLocation().X,
			GetCapsuleComponent()->GetComponentLocation().Y,
			GetCapsuleComponent()->GetComponentLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
			GetCapsuleComponent()->GetCollisionProfileName(),
			false,
			{ this },
			EDrawDebugTrace::None,
			Hit,
			true))
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(Hit.Distance - 0.1f);
	}
	
	GetMesh()->SetWorldRotation(
		FRotator(
			GetMesh()->GetComponentRotation().Pitch,
			Camera->GetComponentRotation().Yaw - 90.0f,
			GetMesh()->GetComponentRotation().Roll));;
}

void AVRCharacter::ApplyHeight()
{
	GetMesh()->SetWorldScale3D(FVector(BaseHeight / 100.0f));
}

void AVRCharacter::CalculateHeight()
{
	float NewBaseHeight = (Camera->GetComponentLocation().Z + GetCapsuleComponent()->GetScaledCapsuleRadius()) - CalculaterFloorLevel();
	BaseHeight = FMath::Clamp(NewBaseHeight, 50.0f, NewBaseHeight);
	ApplyHeight();
}

void AVRCharacter::InitPhysicalAnimation()
{
	GetMesh()->SetAllBodiesBelowSimulatePhysics("clavicle_r", true);
	GetMesh()->SetAllBodiesBelowSimulatePhysics("clavicle_l", true);
	
	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH, this, &AVRCharacter::EnalbePhysicalAnimation, 0.2f);
}

void AVRCharacter::EnalbePhysicalAnimation()
{
	PhysicalAnimation->SetSkeletalMeshComponent(GetMesh());
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow("calvicle_r", GetPhysicalAnimationSettings());
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow("calvicle_l", GetPhysicalAnimationSettings());
}

void AVRCharacter::OnSettingApplied()
{
	LoadSaveGame();
}

void AVRCharacter::LoadSaveGame()
{
	if (UVRSaveGame* SaveGame = UVRSaveGame::LoadVRSaveGameFromSlot(UVRSaveGame::GetDefaultSlotName(), 0))
	{
		TurnStyle = SaveGame->TurnStyle;
		MovementDirection = SaveGame->MovementDirection;
		BaseHeight = SaveGame->BaseHeight;
	}
}

void AVRCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	UpdateMovementSpeed();

	switch (MovementDirection)
	{
	case MD_Controller:
		{
			AddMovementInput(LMC->GetForwardVector().GetSafeNormal2D(), Input.Y);
			AddMovementInput(LMC->GetRightVector().GetSafeNormal2D(), Input.X);
		}
		break;
	case MD_Camera:
		{
			AddMovementInput(Camera->GetForwardVector().GetSafeNormal2D(), Input.Y);
			AddMovementInput(Camera->GetRightVector().GetSafeNormal2D(), Input.X);
		}
		break;
	default:
		{
			break;		
		}
	}
}

void AVRCharacter::StopMoving(const FInputActionValue& Value)
{
	bIsRunning = false;
}

void AVRCharacter::UpdateMovementSpeed()
{
	float CurrentHeightRatio = GetHeightRatio();
	
	if (CurrentHeightRatio <= CrouchRatio)
	{
		GetCharacterMovement()->MaxWalkSpeed = FMath::GetMappedRangeValueClamped<float>(TRange<float>(ProneRatio, CrouchRatio), TRange<float>(BaseWalkSpeed, BaseCrouchSpeed), CurrentHeightRatio);
		bIsRunning = false;
		return;
	}
	
	 if (MoveActionBinding->GetValue().Get<FVector2D>().Y <= 0.0)
	 {
		 GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	 	bIsRunning = false;
	 	return;
	 }
	
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
		return;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AVRCharacter::Turn(const FInputActionValue& Value)
{
	if (TurnStyle == TS_Smooth)
	{
		float Input = Value.Get<float>();
		AddControllerYawInput(Input);
	}
}

void AVRCharacter::SnapTurn(const FInputActionValue& Value)
{
	if (TurnStyle == TS_Snap)
	{
		if (Value.Get<float>() > 0.0f)
		{
			AddControllerYawInput(22.5f);
		}else
		{
			AddControllerYawInput(-22.5f);
		}
	}
}

void AVRCharacter::TryJump(const FInputActionValue& Value)
{
	if (GetHeightRatio() > CrouchRatio && !bIsClimbingRight && bIsClimbingLeft)
	{
		Jump();
	}
}

void AVRCharacter::Run(const FInputActionValue& Value)
{
	if (GetHeightRatio() > CrouchRatio && !bIsClimbingRight && bIsClimbingLeft)
	{
		bIsRunning = true;
	}
}

void AVRCharacter::CalculateCameraVelocity()
{
	CameraVelocity = (Camera->GetComponentLocation() - PreviousCameraLocation) / GetWorld()->GetDeltaSeconds();
	PreviousCameraLocation = Camera->GetComponentLocation();
}

void AVRCharacter::StartGrabbingRight(const FInputActionValue& Value)
{
	if (!RightGrabbedActor && !RightWeaponInterface)
	{
		if (AActor* GrabbedActor = StartGrabbing(RPC, RightHandBone, LeftGrabbedActor, RMC, bIsRightMainHand, bIsLeftMainHand))
		{
			GrabbedActor->SetOwner(this);
			RightGrabbedActor = GrabbedActor;
			
			if (IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(GrabbedActor))
			{
				if (bIsRightMainHand)
				{
					RightWeaponInterface.SetInterface(WeaponInterface);
					RightWeaponInterface.SetObject(GrabbedActor);
					RightWeaponInterface->SetPrimaryHand(EControllerHand::Right);
					
					OnRightHandAnimChanged.Broadcast(RightWeaponInterface->GetPrimaryHandAnimation(), RightWeaponInterface->GetPrimaryHandTriggerPulledAnimation(), true);
				}else
				{
					WeaponInterface->SetSecondaryHand(EControllerHand::Right);
					OnRightHandAnimChanged.Broadcast(WeaponInterface->GetSecondaryHandAnimation(), nullptr, true);
				}
			}
		}else
		{
			StartClimbing(RMC, bIsClimbingRight, bIsClimbingLeft);
		}
	}
}

void AVRCharacter::UpdateGrabbingRight(const FInputActionValue& Value)
{
	UpdateGrabbbing(RightGrabbedActor, bIsRightMainHand, RPC, RMC, RightHandBone);
	UpdateClimbing(RMC, bIsClimbingRight, bIsClimbingLeft);
}

void AVRCharacter::StopGrabbingRight(const FInputActionValue& Value)
{
	StopClimbing(RMC, bIsClimbingRight, bIsClimbingLeft);
	OnRightHandAnimChanged.Broadcast(nullptr, nullptr, false);
	
	if (RightGrabbedActor && !RightGrabbedActor->IsA<AAmmoBase>())
	{
		if (bIsRightMainHand)
		{
			StopGrabbing(RPC, RightGrabbedActor, LeftGrabbedActor);
			RightGrabbedActor = nullptr;
			bIsRightMainHand = false;
			
			if (RightWeaponInterface)
			{
				RightWeaponInterface->StopFiringWeapon();
				RightWeaponInterface->SetPrimaryHand(EControllerHand::AnyHand);
			}
			
			RightWeaponInterface = nullptr;
		}else
		{
			if (!RightWeaponInterface || RightGrabbedActor->IsA<AShotgunBase>())
			{
				StopGrabbing(RPC, RightGrabbedActor, LeftGrabbedActor);
				
				if (IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(RightGrabbedActor))
				{
					WeaponInterface->SetSecondaryHand(EControllerHand::AnyHand);
				}
				
				RightGrabbedActor = nullptr;
			}
		}
	}
}

void AVRCharacter::StartGrabbingLeft(const FInputActionValue& Value)
{
	if (!LeftGrabbedActor && !LeftWeaponInterface)
	{
		if (AActor* GrabbedActor = StartGrabbing(RPC, LeftHandBone, RightGrabbedActor, LMC, bIsLeftMainHand, bIsRightMainHand))
		{
			GrabbedActor->SetOwner(this);
			LeftGrabbedActor = GrabbedActor;
			
			if (IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(GrabbedActor))
			{
				if (bIsLeftMainHand)
				{
					LeftWeaponInterface.SetInterface(WeaponInterface);
					LeftWeaponInterface.SetObject(GrabbedActor);
					LeftWeaponInterface->SetPrimaryHand(EControllerHand::Left);
					
					OnLeftHandAnimChanged.Broadcast(LeftWeaponInterface->GetPrimaryHandAnimation(), LeftWeaponInterface->GetPrimaryHandTriggerPulledAnimation(), true);
				}else
				{
					WeaponInterface->SetSecondaryHand(EControllerHand::Right);
					OnLeftHandAnimChanged.Broadcast(WeaponInterface->GetSecondaryHandAnimation(), nullptr, false);
				}
			}
		}else
		{
			StartClimbing(LMC, bIsClimbingLeft, bIsClimbingRight);
		}
	}
}

void AVRCharacter::UpdateGrabbingLeft(const FInputActionValue& Value)
{
	UpdateGrabbbing(LeftGrabbedActor, bIsLeftMainHand, LPC, LMC, LeftHandBone);
	UpdateClimbing(LMC, bIsClimbingLeft, bIsClimbingRight);
}

void AVRCharacter::StopGrabbingLeft(const FInputActionValue& Value)
{
	StopClimbing(LMC, bIsClimbingLeft, bIsClimbingRight);
	OnLeftHandAnimChanged.Broadcast(nullptr, nullptr, false);
	
	if (LeftGrabbedActor && !LeftGrabbedActor->IsA<AAmmoBase>())
	{
		if (bIsLeftMainHand)
		{
			StopGrabbing(LPC, LeftGrabbedActor, RightGrabbedActor);
			LeftGrabbedActor = nullptr;
			bIsLeftMainHand = false;
			
			if (LeftWeaponInterface)
			{
				LeftWeaponInterface->StopFiringWeapon();
				LeftWeaponInterface->SetPrimaryHand(EControllerHand::AnyHand);
			}
			
			LeftWeaponInterface = nullptr;
		}else
		{
			if (!LeftWeaponInterface || LeftGrabbedActor->IsA<AShotgunBase>())
			{
				StopGrabbing(LPC, LeftGrabbedActor, RightGrabbedActor);
				
				if (IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(LeftGrabbedActor))
				{
					WeaponInterface->SetSecondaryHand(EControllerHand::AnyHand);
				}
				
				LeftGrabbedActor = nullptr;
			}
		}
	}
}

AActor* AVRCharacter::StartGrabbing(UPhysicsConstraintComponent* PC, FName BoneToConstrainTo, AActor* OtherGrabbedActor,
	UMotionControllerComponent* MC, bool& bIsMainHand, bool bIsOtherMainHand)
{
	FHitResult Hit = TraceForObjects(MC, GrabbleObjectTypes);
	AActor* HitActor  = Hit.GetActor();
	
	if (HitActor && !HitActor->IsA<AAmmoBase>())
	{
		if (AWeaponBase* weapon = Cast<AWeaponBase>(HitActor->GetOwner()))
		{
			HitActor = weapon;
		}
		
		if (UPrimitiveComponent* ComponentToGrab = Cast<UPrimitiveComponent>(HitActor->GetRootComponent()))
		{
			if (HitActor == OtherGrabbedActor && bIsOtherMainHand)
			{
				bIsMainHand = false;
				
				if (AShotgunBase* Shotgun = Cast<AShotgunBase>(HitActor))
				{
					if (Shotgun->SelectedFiringMode == FM_Single)
					{
						if (IWeaponInterface* WI = Cast<IWeaponInterface>(Shotgun))
						{
							if (WI->GetSlideComponent())
							{
								PC->SetConstrainedComponents(GetMesh(), BoneToConstrainTo, WI->GetSlideComponent(), GetGripBoneName(BoneToConstrainTo));
								PC->SetConstraintReferenceFrame(EConstraintFrame::Frame1, WI->GetSlideComponent()->GetSocketTransform(GetGripBoneName(BoneToConstrainTo), RTS_Component));
								PC->SetConstraintReferenceFrame(EConstraintFrame::Frame2, FTransform());
								
								WI->StartMovingSlide(MC);
								return Shotgun;
							}
						}
					}
				}
				
				PC->SetConstrainedComponents(GetMesh(), BoneToConstrainTo, ComponentToGrab, GetGripBoneName(BoneToConstrainTo));
				
				if (ComponentToGrab->DoesSocketExist(GetGripBoneName(BoneToConstrainTo)))
				{
					PC->SetConstraintReferenceFrame(EConstraintFrame::Frame1, ComponentToGrab->GetSocketTransform(GetGripBoneName(BoneToConstrainTo), RTS_Component));
					PC->SetConstraintReferenceFrame(EConstraintFrame::Frame2, FTransform());
				}
				
				return HitActor;
			}
			
			bIsMainHand = true;
			PC->SetConstrainedComponents(GetMesh(), BoneToConstrainTo,ComponentToGrab , GetHandleBoneName(BoneToConstrainTo));
			
			if (ComponentToGrab->DoesSocketExist(GetHandleBoneName(BoneToConstrainTo)))
			{
				PC->SetConstraintReferenceFrame(EConstraintFrame::Frame1, ComponentToGrab->GetSocketTransform(GetHandleBoneName(BoneToConstrainTo), RTS_Component));
				PC->SetConstraintReferenceFrame(EConstraintFrame::Frame2, FTransform());
			}
			return HitActor;
		}
	}
	return nullptr;
}

void AVRCharacter::UpdateGrabbbing(AActor* GrabbedActor, bool bIsMainHand, UPhysicsConstraintComponent* PC,
	UMotionControllerComponent* MC, FName BoneToConstrainTo)
{
	if (!bIsMainHand)
	{
		if (AShotgunBase* Shotgun = Cast<AShotgunBase>(GrabbedActor))
		{
			if (Shotgun->SelectedFiringMode == FM_Single)
			{
				if (IWeaponInterface* WI = Cast<IWeaponInterface>(Shotgun))
				{
					if (WI->GetSlideComponent())
					{
						PC->SetConstraintReferencePosition(EConstraintFrame::Frame2, WI->GetSlideComponent()->GetRelativeLocation());
						WI->MoveSlide(MC, GetGripBoneName(BoneToConstrainTo));
					}
				}
			}
		}
	}
}

void AVRCharacter::StopGrabbing(UPhysicsConstraintComponent* PC, AActor* GrabbedActor, AActor* OtherGrabbedActor)
{
	PC->BreakConstraint();
	
	if (GrabbedActor && GrabbedActor != OtherGrabbedActor)
	{
		GrabbedActor->SetOwner(nullptr);
	}
}

void AVRCharacter::ReleaseGrabbedActor(AActor* GrabbedActor)
{
	if (GrabbedActor == RightGrabbedActor)
	{
		RPC->BreakConstraint();
		RightGrabbedActor = nullptr;
		bIsRightMainHand = false;
		return;
	}
	
	if (GrabbedActor == LeftGrabbedActor)
	{
		LPC->BreakConstraint();
		LeftGrabbedActor = nullptr;
		bIsLeftMainHand = false;
		return;
	}
}

FHitResult AVRCharacter::TraceForObjects(UMotionControllerComponent* MC, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes)
{
	FHitResult Hit;
	UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		MC->GetComponentLocation(), MC->GetComponentLocation() + FVector(0.0f, 0.0f, 1.0f),
		GrabRadius,
		ObjectTypes,
		false,
		{ this },
		EDrawDebugTrace::None,
		Hit,
		true);
	return Hit;
}

void AVRCharacter::InteractRight(const FInputActionValue& Value)
{
	if (!StartWeaponInteraction(RightGrabbedActor, RightWeaponInterface, RMC, RightHandBone, RPC))
	{
		StartAmmoBoxInteraction(RMC, RightGrabbedActor, RightHandBone);
	}
	
	if (!RightGrabbedActor)
	{
		if (RightWeaponInterface)
		{
			OnRightHandAnimChanged.Broadcast(RightWeaponInterface->GetSlideGripAnimation(), nullptr, true);
			return;
		}
		
		WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
	}
}

void AVRCharacter::UpdateInteractRight(const FInputActionValue& Value)
{
	UpdateWeaponInteraction(RightWeaponInterface, bIsRightMainHand, RMC, RightHandBone, RPC);
}

void AVRCharacter::StopInteractRight(const FInputActionValue& Value)
{
	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
	
	if (!StopWeaponInteraction(RightWeaponInterface, bIsRightMainHand, RightGrabbedActor, RPC))
	{
		StopAmmoBoxInteraction(RightGrabbedActor);
		return;
	}
	
	if (!bIsRightMainHand)
	{
		OnRightHandAnimChanged.Broadcast(nullptr, nullptr, false);
		RightWeaponInterface = nullptr;
	}
}

void AVRCharacter::InteractLeft(const FInputActionValue& Value)
{
	if (!StartWeaponInteraction(LeftGrabbedActor, LeftWeaponInterface, LMC, LeftHandBone, LPC))
	{
		StartAmmoBoxInteraction(LMC, LeftGrabbedActor, LeftAmmoBone);
	}
	
	if (!LeftGrabbedActor && LeftWeaponInterface)
	{
		OnLeftHandAnimChanged.Broadcast(LeftWeaponInterface->GetSlideGripAnimation(), nullptr, false);
	}
}

void AVRCharacter::UpdateInteractLeft(const FInputActionValue& Value)
{
	UpdateWeaponInteraction(LeftWeaponInterface, bIsLeftMainHand, LMC, LeftHandBone, LPC);
}

void AVRCharacter::StopInteractLeft(const FInputActionValue& Value)
{
	if (!StopWeaponInteraction(LeftWeaponInterface, bIsLeftMainHand, LeftGrabbedActor, LPC))
	{
		StopAmmoBoxInteraction(LeftGrabbedActor);
		return;
	}
	
	if (!bIsLeftMainHand)
	{
		OnLeftHandAnimChanged.Broadcast(nullptr, nullptr, false);
		LeftWeaponInterface = nullptr;
	}
}

void AVRCharacter::ReleaseMagRight(const FInputActionValue& Value)
{
	if (bIsRightMainHand && RightGrabbedActor && RightWeaponInterface)
	{
		RightWeaponInterface->ReleaseMag();
	}
}

void AVRCharacter::ReleaseMagLeft(const FInputActionValue& Value)
{
	if (bIsLeftMainHand && LeftGrabbedActor && LeftWeaponInterface)
	{
		LeftWeaponInterface->ReleaseMag();
	}
}

void AVRCharacter::ChangeFiringModeRight(const FInputActionValue& Value)
{
	if (bIsRightMainHand && RightGrabbedActor && RightWeaponInterface)
	{
		RightWeaponInterface->ChangeFiringMode();
	}
}

void AVRCharacter::ChangeFiringModeLeft(const FInputActionValue& Value)
{
	if (bIsLeftMainHand && LeftGrabbedActor && LeftWeaponInterface)
	{
		LeftWeaponInterface->ChangeFiringMode();
	}
}

void AVRCharacter::ApplyRecoil(EControllerHand Hand, FVector LocationOffset, float Pitch)
{
	switch (Hand)
	{
	case EControllerHand::Left:
		{
			LeftRecoilOffset = LocationOffset;
			LeftRecoilPitch = Pitch;
			
			if (!LeftRecoilTH.IsValid())
			{
				GetWorldTimerManager().SetTimer(LeftRecoilTH,
					FTimerDelegate::CreateLambda([this]()
						{
							this->LerpRecoil(LeftRecoilOffset, LeftRecoilPitch, LeftRecoilTH);
						}),
						RecoilLerpTimerInterval,
						true);
			}
			
			break;
		}
		case EControllerHand::Right:
		{
			RightRecoilOffset = LocationOffset;
			RightRecoilPitch = Pitch;
			if (!RightRecoilTH.IsValid())
			{
				GetWorldTimerManager().SetTimer(RightRecoilTH,
					FTimerDelegate::CreateLambda([this]()
					{
						this->LerpRecoil(RightRecoilOffset, RightRecoilPitch, RightRecoilTH);
					}),
					RecoilLerpTimerInterval,
					true);
			}
			break;;
		}
	default:
		break;
	}
}

void AVRCharacter::LerpRecoil(FVector& LocationOffset, float& Pitch, FTimerHandle& RecoilTH)
{
	LocationOffset = FMath::Lerp(LocationOffset, FVector::ZeroVector, RecoilLerpAplha);
	Pitch = FMath::Lerp(Pitch, 0.0f, RecoilLerpAplha);
	
	if (LocationOffset.IsNearlyZero(0.1f) && FMath::IsNearlyZero(Pitch, 0.1f))
	{
		LocationOffset = FVector::ZeroVector;
		Pitch = 0.0f;
		GetWorldTimerManager().ClearTimer(RecoilTH);
	}
}

bool AVRCharacter::CheckForSlide(AActor* Actor, TScriptInterface<IWeaponInterface>& WI, FName BoneToConstrainTo,
	UPhysicsConstraintComponent* PC, UMotionControllerComponent* MC)
{
	if (Actor)
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(Actor->GetOwner()))
		{
			Actor = Weapon;
		}
		
		if (IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(Actor))
		{
			WI.SetInterface(WeaponInterface);
			WI.SetObject(Actor);
			
			if (WI->GetSlideComponent())
			{
				PC->SetConstrainedComponents(GetMesh(), BoneToConstrainTo, WI->GetSlideComponent(), GetGripBoneName(BoneToConstrainTo));
				
				if (WI->GetSlideComponent()->DoesSocketExist(GetGripBoneName(BoneToConstrainTo)))
				{
					PC->SetConstraintReferenceFrame(EConstraintFrame::Frame1, WI->GetSlideComponent()->GetSocketTransform(GetGripBoneName(BoneToConstrainTo), RTS_Component));
					PC->SetConstraintReferenceFrame(EConstraintFrame::Frame2, FTransform());
				}
				
				WI->StartMovingSlide(MC);
				return true;
			}
		}
	}
	return false;
}

FName AVRCharacter::GetGripBoneName(FName HandBoneName)
{
	return FName("grip_" + HandBoneName.ToString());
}

FName AVRCharacter::GetHandleBoneName(FName HandBoneName)
{
	return FName("handle_" + HandBoneName.ToString());
}

bool AVRCharacter::StartWeaponInteraction(AActor*& GrabbedActor, TScriptInterface<IWeaponInterface>& WI,
	 UMotionControllerComponent* MC, FName BoneToConstrainTo, UPhysicsConstraintComponent* PC)
{
	if (GrabbedActor && WI)
	{
		WI->FireWeapon();
		return true;
	}
	
	if (!GrabbedActor && WI)
	{
		FHitResult Hit = TraceForObjects(MC, GrabbleObjectTypes);
		
		if (AShotgunBase* Shotgun = Cast<AShotgunBase>(Hit.GetActor()))
		{
			if (Shotgun->SelectedFiringMode == FM_Single)
			{
				return false;
			}
		}
		
		return CheckForSlide(Hit.GetActor(), WI, BoneToConstrainTo, PC, MC);
	}
	return false;
}

bool AVRCharacter::UpdateWeaponInteraction(TScriptInterface<IWeaponInterface> WI, bool bIsMainHand,
	UMotionControllerComponent* MC, FName BoneToConstrainTo, UPhysicsConstraintComponent* PC)
{
	if (WI && !bIsMainHand && WI->GetSlideComponent())
	{
		WI->MoveSlide(MC, GetGripBoneName(BoneToConstrainTo));
		PC->SetConstraintReferencePosition(EConstraintFrame::Frame2, WI->GetSlideComponent()->GetRelativeLocation());
		
		return true;
	}
	return false;
}

bool AVRCharacter::StopWeaponInteraction(TScriptInterface<IWeaponInterface>& WI, bool bIsMainHand, AActor* GrabbedActor,
	UPhysicsConstraintComponent* PC)
{
	if (WI)
	{
		if (!GrabbedActor)
		{
			PC->BreakConstraint();
			WI->StopMovingSlide();
			return true;
		}
		
		WI->StopFiringWeapon();
		return true;
	}
	
	return false;
}

void AVRCharacter::StartAmmoBoxInteraction(UMotionControllerComponent* MC, AActor*& GrabbedActor, FName BoneToAttachTo)
{
	if (!GrabbedActor)
	{
		FHitResult Hit = TraceForObjects(MC, GrabbleObjectTypes);
		
		if (AAmmoBoxBase* AmmoBox = Cast<AAmmoBoxBase>(Hit.GetActor()))
		{
			GrabbedActor = AmmoBox->TakeAmmo(GetMesh(), BoneToAttachTo);
		}
	}
}

void AVRCharacter::StopAmmoBoxInteraction(AActor*& GrabbedActor)
{
	if (AAmmoBase* Ammo = Cast<AAmmoBase>(GrabbedActor))
	{
		if (AAmmoBoxBase* AmmoBox = Cast<AAmmoBoxBase>(Ammo->GetOwner()))
		{
			AmmoBox->ReturnAmmo();
		}else
		{
			Ammo->Destroy();
		}
		
		GrabbedActor = nullptr;
	}
}

void AVRCharacter::StartClimbing(UMotionControllerComponent* MC, bool& bIsClimbing, bool& bIsOtherClimbing)
{
	if (MC)
	{
		FHitResult Hit = TraceForObjects(MC, ClimbableObjectTypes);
		
		if (Hit.bBlockingHit)
		{
			CurrentSlippingSpeed = 0.0f;
			
			bIsClimbing = true;
			bIsOtherClimbing = false;
			
			MCStartLocation = MC->GetComponentLocation();
			
			if (Hit.GetComponent())
			{
				SetClimbingOffset(Hit);
			}
			
			GetCharacterMovement()->MaxStepHeight = 0.0f;
			GetCharacterMovement()->GravityScale = 0.0f;
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			GetCharacterMovement()->UpdateComponentVelocity();
			
			PreviousSlidingHit = Hit;
			SlidingStartNormal = Hit.ImpactNormal;
		}
	}
}

void AVRCharacter::UpdateClimbing(UMotionControllerComponent* MC, bool bIsClimbing, bool bIsOtherClimbing)
{
	if (MC && bIsClimbing)
	{
		AddActorWorldOffset(MCStartLocation - MC->GetComponentLocation(), true);
		
		if (!bCanSlipWhileClimbing || (PreviousSlidingHit.PhysMaterial.IsValid() && PreviousSlidingHit.PhysMaterial.Get()->Friction > 1.0f))
		{
			UpdateClimbingOffSet(PreviousSlidingHit);
		}else if (!CheckSlipping(MC))
		{
			StopClimbing(MC, bIsClimbing, bIsOtherClimbing);
		}
		
		PreviousMcLocation = MC->GetComponentLocation();
	}
}

void AVRCharacter::StopClimbing(UMotionControllerComponent* MC, bool& bIsClimbing, bool bIsOtherClimbing)
{
	if (MC && !bIsOtherClimbing && bIsClimbing)
	{
		GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->MaxStepHeight = BaseStepHeight;
		
		if (bCanLaunchCharacter)
		{
			GetCharacterMovement()->Velocity = UKismetMathLibrary::ClampVectorSize((PreviousMcLocation - MC->GetComponentLocation()) / GetWorld()->DeltaRealTimeSeconds, 0.0f, 600.0f);
			GetCharacterMovement()->UpdateComponentVelocity();
		}
	}
	
	bIsClimbing = false;
}

bool AVRCharacter::CheckSlipping(UMotionControllerComponent* MC)
{
	if (MC)
	{
		FHitResult Hit = TraceForObjects(MC, ClimbableObjectTypes);
		
		if (Hit.bBlockingHit)
		{
			if (!FMath::IsNearlyEqual(Hit.ImpactNormal.Z, PreviousSlidingHit.ImpactNormal.Z, 0.005f))
			{
				return CheckSecondarySlipping(MC);
			}
			
			if (Hit.ImpactNormal.Z + 0.0001 < 1.0f)
			{
				MCStartLocation += UpdateSlipping(Hit);
			}
			
			UpdateClimbingOffSet(Hit);
			
			PreviousSlidingHit = Hit;
			return true;
		}
		return CheckSecondarySlipping(MC);
	}
	return false;
}

bool AVRCharacter::CheckSecondarySlipping(UMotionControllerComponent* MC)
{
	if (MC)
	{
		FHitResult SecondHit;
		FRotator ImpactRotation = SlidingStartNormal.Rotation() + FRotator(-90.0f, 0.0f, 0.0f);
		FVector Start = MCStartLocation + ImpactRotation.Vector() * (GrabRadius * 2.0f);
		
		if (UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			Start,
			Start + PreviousSlidingHit.ImpactNormal * -(GrabRadius * 5.0f),
			ClimbableObjectTypes,
			false,
			{ this },
			EDrawDebugTrace::None,
			SecondHit,
			true))
		{
			if (SecondHit.ImpactNormal.Z + 0.0001f < 1.0f)
			{
				MCStartLocation += UpdateSlipping(SecondHit);
				
				UpdateClimbingOffSet(SecondHit);
				
				PreviousSlidingHit = SecondHit;
				SlidingStartNormal = SecondHit.ImpactNormal;
			}
			
			return true;
		}
		
	}
	return false;
}

void AVRCharacter::SetClimbingOffset(const FHitResult& Hit)
{
	if (Hit.GetComponent())
	{
		ClimbingOffset = MCStartLocation - Hit.GetComponent()->GetSocketLocation(Hit.BoneName);
		LastClimbedActorQuat = Hit.GetComponent()->GetSocketQuaternion(Hit.BoneName);
	}
}

void AVRCharacter::UpdateClimbingOffSet(const FHitResult& Hit)
{
	if (Hit.GetComponent())
	{
		if (Hit.GetComponent() != PreviousSlidingHit.GetComponent())
		{
			SetClimbingOffset(Hit);
		}
		
		FQuat QuatOffset = Hit.GetComponent()->GetSocketQuaternion(Hit.BoneName) * LastClimbedActorQuat.Inverse();
		ClimbingOffset = QuatOffset.RotateVector(ClimbingOffset);
		
		QuatOffset.X = 0.0f;
		QuatOffset.Y = 0.0f;
		
		VROrigin->AddWorldRotation(QuatOffset, true);
		
		MCStartLocation = (Hit.GetComponent()->GetSocketLocation(Hit.BoneName) + ClimbingOffset) - MCStartLocation;
		LastClimbedActorQuat = Hit.GetComponent()->GetSocketQuaternion(Hit.BoneName);
	}
}

FVector AVRCharacter::UpdateSlipping(const FHitResult& Hit)
{
	if (Hit.PhysMaterial.IsValid())
	{
		float ClampedFriction = 1.0f - FMath::Clamp(Hit.PhysMaterial.Get()->Friction, 0.0f, 1.0f);
		float SlipSpeed =ClampedFriction * CurrentSlippingSpeed * GetWorld()->DeltaRealTimeSeconds;
		
		FVector SlipVelocity = FVector(0.0f, 0.0f, SlipSpeed);
		
		float RoundedNormal = Hit.ImpactNormal.Z;
		
		if (RoundedNormal > 0.0f)
		{
			RoundedNormal += 0.0001f;
		}else
		{
			RoundedNormal -= 0.0001f;
		}
		
		if ((RoundedNormal < 1.0f && RoundedNormal > 0.0f) || (bCanSlideDownOverhang && RoundedNormal < 0.0f && RoundedNormal > -0.0f))
		{
			FRotator ImpactRotation = Hit.ImpactNormal.Rotation() + FRotator(-90.0f, 0.0f, 0.0f);
			SlipVelocity = -ImpactRotation.Vector() * SlipSpeed * (1.0f - Hit.ImpactNormal.Z);
			
			CurrentSlippingSpeed = FMath::Lerp(CurrentSlippingSpeed, GetWorld()->GetGravityZ(), GetWorld()->DeltaRealTimeSeconds * 2.0f);
			AddActorWorldOffset(SlipVelocity, true);
			
			ClimbingOffset += SlipVelocity;
			return SlipVelocity;
		}
	}
	return FVector::ZeroVector;
}

void AVRCharacter::LerpClimbingBodyZOffset(float TargetOffset, float TargetHalfHeight)
{
	ClimbingbodyZOffset = FMath::Lerp(ClimbingbodyZOffset, TargetOffset, GetWorld()->GetDeltaSeconds() * ClimbingOffsetLerpSpeed);
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), TargetHalfHeight, GetWorld()->GetDeltaSeconds() * ClimbingOffsetLerpSpeed));
}

void AVRCharacter::ThumbRightAxis(const FInputActionValue& Value)
{
	RightThumbAxisValue = Value.Get<bool>();
}

void AVRCharacter::ThumbLeftAxis(const FInputActionValue& Value)
{
	LeftThumbAxisValue = Value.Get<bool>();
}

void AVRCharacter::IndexRightAxis(const FInputActionValue& Value)
{
	RightIndexAxisValue = Value.Get<bool>();
	
	if (RightWeaponInterface && bIsRightMainHand)
	{
		RightWeaponInterface->PullTrigger(RightIndexAxisValue);
	}
}

void AVRCharacter::IndexLeftAxis(const FInputActionValue& Value)
{
	LeftIndexAxisValue = Value.Get<bool>();
	
	if (LeftWeaponInterface && bIsLeftMainHand)
	{
		LeftWeaponInterface->PullTrigger(LeftIndexAxisValue);
	}
}

void AVRCharacter::GripRightAxis(const FInputActionValue& Value)
{
	RightGripAxisValue = Value.Get<bool>();
}

void AVRCharacter::GripLeftAxis(const FInputActionValue& Value)
{
	LeftGripAxisValue = Value.Get<bool>();
}

