// Fill out your copyright notice in the Description page of Project Settings.


#include "VRSaveGame.h"
#include <Kismet/GameplayStatics.h>

UVRSaveGame::UVRSaveGame()
{
}

UVRSaveGame* UVRSaveGame::LoadVRSaveGameFromSlot(const FString& SlotName, const int32 UserIndex)
{
	UVRSaveGame* SaveGame = Cast<UVRSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	
	if (!SaveGame)
	{
		SaveGame = NewObject<UVRSaveGame>();
		UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
	}
	return SaveGame;
}

bool UVRSaveGame::SaveVRSaveGameToSlot(UVRSaveGame* SaveGame, const FString& SlotName, const int32 UserIndex)
{
	if (SaveGame)
	{
		return UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
	}
	return false;
}
