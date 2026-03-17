// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingsMenuWidget.h"
#include "VRSaveGame.h"
#include "Widgets/AudioSettingsWidget.h"
#include "Widgets/GeneralSettingWidget.h"
#include "Widgets/GraphicsSettingWidget.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <GameFramework/GameUserSettings.h>

void USettingsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UVRSaveGame* SaveGame = UVRSaveGame::LoadVRSaveGameFromSlot(UVRSaveGame::GetDefaultSlotName(), 0))
	{
		GeneralSettingsWidget->CurrentTurnStyle - SaveGame->BaseHeight;
		GeneralSettingsWidget->CurrentMovementDirection = SaveGame->MovementDirection;
		
		AudioSettingsWidget->CurrentMasterVolume = SaveGame->MasterVolume;
		AudioSettingsWidget->ApplyAudioSettings();
	}
}

void USettingsMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	GeneralButton->OnPressed.AddDynamic(this, &USettingsMenuWidget::OpenGeneralSettings);
	GraphicsButton->OnPressed.AddDynamic(this, &USettingsMenuWidget::OpenGraphicsSettings);
	AudioButton->OnPressed.AddDynamic(this, &USettingsMenuWidget::OpenAudioSettings);
	
	GeneralSettingsWidget->OnSettingsChanged.AddDynamic(this, &USettingsMenuWidget::ApplySettings);
	AudioSettingsWidget->OnSettingsChanged.AddDynamic(this, &USettingsMenuWidget::ApplySettings);
}

void USettingsMenuWidget::ApplySettings()
{
	OnSettingsApplied.Broadcast();
}

void USettingsMenuWidget::OpenGeneralSettings()
{
	WidgetSwitcher->SetActiveWidget(GeneralSettingsWidget);
}

void USettingsMenuWidget::OpenGraphicsSettings()
{
	WidgetSwitcher->SetActiveWidget(GraphicsSettingsWidget);
}

void USettingsMenuWidget::OpenAudioSettings()
{
	WidgetSwitcher->SetActiveWidget(AudioSettingsWidget);
}
