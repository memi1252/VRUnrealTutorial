// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioSettingsWidget.generated.h"

class USlider;
class USoundMix;
class USoundClass;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAudioSettingsChangedDelegate);

UCLASS()
class VRTUTORIAL_API UAudioSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
#pragma region Variables
	
	UPROPERTY(meta = (BindWidget))
	USlider* MasterVolumeSlider;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	float CurrentMasterVolume = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	USoundMix* SoundMixModifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	USoundClass* SoundClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Varialbes")
	FOnAudioSettingsChangedDelegate OnSettingsChanged;
	
#pragma endregion
	
#pragma region Functions
	
	virtual void NativeConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	float GetMasterVolumeValue();
	
	UFUNCTION()
	void OnMasterVolumeChanged(float value);
	
	UFUNCTION()
	void OnSlideLetGo();
	
	void ApplyAudioSettings();
	
#pragma endregion
};
