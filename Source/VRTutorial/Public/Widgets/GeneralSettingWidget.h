// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GeneralSettingWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGeneralSettingsChangedDelegate);

UCLASS()
class VRTUTORIAL_API UGeneralSettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
#pragma region Variables
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	FOnGeneralSettingsChangedDelegate OnSettingsChanged;
	
	UPROPERTY(meta = (BindWidget))
	UButton* PreviousTurnStyleButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* NextTurnStyleButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnStyleText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	TEnumAsByte<ETurnStyle> CurrentTurnStyle;
	
	UPROPERTY(meta = (BindWidget))
	UButton* PreviousMovementDirectionButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* NextMovementDirectionButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MovementDirectionText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	TEnumAsByte<EMovementDirection> CurrentMovementDirection;
	
#pragma endregion
	
#pragma region Functions
	
	virtual void NativeConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
	void SaveGeneralSettings();
	
	UFUNCTION(BlueprintCallable)
	void NextTurnStyle();
	
	UFUNCTION(BlueprintCallable)
	void PreviousTurnStyle();
	
	UFUNCTION(BlueprintCallable)
	FText GetTurnStyleText();
	
	UFUNCTION(BlueprintCallable)
	void NextMovementDirection();
	
	UFUNCTION(BlueprintCallable)
	void PreviousMovementDirection();
	
	UFUNCTION(BlueprintCallable)
	FText GetMovementDirectionText();
	
#pragma endregion
};
