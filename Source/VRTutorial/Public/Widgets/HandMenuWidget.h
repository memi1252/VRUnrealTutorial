// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HandMenuWidget.generated.h"

class UButton;
class AVRCharacter;
class UWidgetSwitcher;
class USettingsMenuWidget;

UCLASS()
class VRTUTORIAL_API UHandMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
#pragma region Variables
	
	AVRCharacter* PlayerRef;
	
	UPROPERTY(meta = (BindWidget))
	UButton* AutoHeighButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	USettingsMenuWidget* SettingsMenuWidget;
	
#pragma endregion
	
#pragma region Functions
	
	virtual void NativeConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void RecalculateHeight();
	
	UFUNCTION(BlueprintCallable)
	void OpenSettingsMenu();
	
	UFUNCTION(BlueprintCallable)
	void Quit();
	
	UFUNCTION(BlueprintCallable)
	void ReturnToHandMenu();
	
#pragma endregion
	
};
