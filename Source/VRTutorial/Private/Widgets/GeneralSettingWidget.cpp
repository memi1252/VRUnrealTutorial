// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GeneralSettingWidget.h"
#include "VRSaveGame.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

void UGeneralSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGeneralSettingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	NextTurnStyleButton->OnPressed.AddDynamic(this, &UGeneralSettingWidget::NextTurnStyle);
	PreviousTurnStyleButton->OnPressed.AddDynamic(this, &UGeneralSettingWidget::PreviousTurnStyle);
	NextMovementDirectionButton->OnPressed.AddDynamic(this, &UGeneralSettingWidget::NextMovementDirection);
	PreviousMovementDirectionButton->OnPressed.AddDynamic(this, &UGeneralSettingWidget::PreviousMovementDirection);
	
	TurnStyleText->TextDelegate.BindUFunction(this, "GetTurnStyleText");
	MovementDirectionText->TextDelegate.BindUFunction(this, "GetMovementDirectionText");
}

void UGeneralSettingWidget::SaveGeneralSettings()
{
	if (UVRSaveGame* SaveGame = UVRSaveGame::LoadVRSaveGameFromSlot(UVRSaveGame::GetDefaultSlotName(), 0))
	{
		SaveGame->TurnStyle = CurrentTurnStyle;
		SaveGame->MovementDirection = CurrentMovementDirection;
		
		UVRSaveGame::SaveVRSaveGameToSlot(SaveGame, UVRSaveGame::GetDefaultSlotName(), 0);
		
		OnSettingsChanged.Broadcast();
	}
}

void UGeneralSettingWidget::NextTurnStyle()
{
	if (CurrentTurnStyle + 1 >= TSMAX)
	{
		CurrentTurnStyle = ETurnStyle(0);
	}else
	{
		CurrentTurnStyle = ETurnStyle(CurrentTurnStyle + 1);
	}
	SaveGeneralSettings();
}

void UGeneralSettingWidget::PreviousTurnStyle()
{
	if (CurrentTurnStyle <= 0)
	{
		CurrentTurnStyle = ETurnStyle(TSMAX -1);
	}else
	{
		CurrentTurnStyle = ETurnStyle(CurrentTurnStyle - 1);
	}
	SaveGeneralSettings();
}

FText UGeneralSettingWidget::GetTurnStyleText()
{
	return UEnum::GetDisplayValueAsText(CurrentTurnStyle);
}

void UGeneralSettingWidget::NextMovementDirection()
{
	if (CurrentMovementDirection  + 1 >= MDMAX)
	{
		CurrentMovementDirection = EMovementDirection(0);
	}else
	{
		CurrentMovementDirection = EMovementDirection(CurrentMovementDirection + 1);
	}
	SaveGeneralSettings();
}

void UGeneralSettingWidget::PreviousMovementDirection()
{
	if (CurrentMovementDirection <= 0)
	{
		CurrentMovementDirection = EMovementDirection(MDMAX -1);
	}else
	{
		CurrentMovementDirection = EMovementDirection(CurrentMovementDirection - 1);
	}
}

FText UGeneralSettingWidget::GetMovementDirectionText()
{
	return UEnum::GetDisplayValueAsText(CurrentMovementDirection);
}
