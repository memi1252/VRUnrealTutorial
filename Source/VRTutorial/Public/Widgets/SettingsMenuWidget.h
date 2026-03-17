// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuWidget.generated.h"

class UButton;
class UVRSaveGame;
class UWidgetSwitcher;
class UAudioSettingsWidget;
class UGeneralSettingWidget;
class UGraphicsSettingWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsAppliedDelegate);

UCLASS()
class VRTUTORIAL_API USettingsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
#pragma region Variables
	
	UPROPERTY(meta = (bindWidget))
	UButton* BackButton;
	
	UPROPERTY(meta = (bineWidget))
	UButton* GeneralButton;
	
	UPROPERTY(meta = (bindWidget))
	UButton* GraphicsButton;
	
	UPROPERTY(meta = (bindWidget))
	UButton* AudioButton;
	
	UPROPERTY(meta = (bindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (bindWidget))
	UGeneralSettingWidget* GeneralSettingsWidget;
	
	UPROPERTY(meta = (bindWidget))
	UGraphicsSettingWidget* GraphicsSettingsWidget;
	
	UPROPERTY(meta = (bindWidget))
	UAudioSettingsWidget* AudioSettingsWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	FOnSettingsAppliedDelegate OnSettingsApplied;
	
#pragma endregion
	
#pragma region Functions
	
	virtual void NativeConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void ApplySettings();
	
	UFUNCTION(BlueprintCallable)
	void OpenGeneralSettings();
	
	UFUNCTION(BlueprintCallable)
	void OpenGraphicsSettings();
	
	UFUNCTION(BlueprintCallable)
	void OpenAudioSettings();
	
#pragma endregion
};
