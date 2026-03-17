// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HandMenuWidget.h"
#include "VRSaveGame.h"
#include "VRCharacter.h"
#include "Widgets/SettingsMenuWidget.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Kismet/KismetSystemLibrary.h>

void UHandMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GetOwningPlayer())
	{
		PlayerRef = Cast<AVRCharacter>(GetOwningPlayer()->GetPawn());
	}
}

void UHandMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	AutoHeighButton->OnPressed.AddDynamic(this, &UHandMenuWidget::RecalculateHeight);
	SettingsButton->OnPressed.AddDynamic(this, &UHandMenuWidget::OpenSettingsMenu);
	QuitButton->OnPressed.AddDynamic(this, &UHandMenuWidget::Quit);
	
	SettingsMenuWidget->BackButton->OnPressed.AddDynamic(this, &UHandMenuWidget::ReturnToHandMenu);
}

void UHandMenuWidget::RecalculateHeight()
{
	if (PlayerRef)
	{
		PlayerRef->CalculateHeight();
		
		if (UVRSaveGame* SaveGame = UVRSaveGame::LoadVRSaveGameFromSlot(UVRSaveGame::GetDefaultSlotName(), 0))
		{
			SaveGame->BaseHeight = PlayerRef->BaseHeight;
			UVRSaveGame::SaveVRSaveGameToSlot(SaveGame, UVRSaveGame::GetDefaultSlotName(), 0);
		}
	}
}

void UHandMenuWidget::OpenSettingsMenu()
{
	WidgetSwitcher->SetActiveWidget(SettingsMenuWidget);
}

void UHandMenuWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UHandMenuWidget::ReturnToHandMenu()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}
