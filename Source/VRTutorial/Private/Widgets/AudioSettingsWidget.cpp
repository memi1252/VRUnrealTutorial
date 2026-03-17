// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AudioSettingsWidget.h"
#include "VRSaveGame.h"

#include <Sound/SoundMix.h>
#include <Sound/SoundClass.h>
#include <Components/Slider.h>
#include <Kismet/GameplayStatics.h>

void UAudioSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAudioSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

float UAudioSettingsWidget::GetMasterVolumeValue()
{
	return CurrentMasterVolume;
}

void UAudioSettingsWidget::OnMasterVolumeChanged(float value)
{
	CurrentMasterVolume = value;
	ApplyAudioSettings();
}

void UAudioSettingsWidget::OnSlideLetGo()
{
	if (UVRSaveGame* SaveGame = UVRSaveGame::LoadVRSaveGameFromSlot(UVRSaveGame::GetDefaultSlotName(), 0))
	{
		SaveGame->MasterVolume = CurrentMasterVolume;
		UVRSaveGame::SaveVRSaveGameToSlot(SaveGame, UVRSaveGame::GetDefaultSlotName(), 0);
		OnSettingsChanged.Broadcast();
	}
}

void UAudioSettingsWidget::ApplyAudioSettings()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMixModifier, SoundClass, CurrentMasterVolume);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMixModifier);
}
