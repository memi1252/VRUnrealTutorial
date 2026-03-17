// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRCharacter.h"
#include "GameFramework/SaveGame.h"
#include "VRSaveGame.generated.h"


UCLASS()
class VRTUTORIAL_API UVRSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
#pragma region Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	TEnumAsByte<ETurnStyle> TurnStyle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	TEnumAsByte<EMovementDirection> MovementDirection;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	float MasterVolume = 1.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	float BaseHeight = 100.0f;
	
#pragma endregion
	
#pragma region Functions
	
	UVRSaveGame();
	
	UFUNCTION(BlueprintCallable)
	static UVRSaveGame* LoadVRSaveGameFromSlot(const FString& SlotName, const int32 UserIndex);
	
	UFUNCTION(BlueprintCallable)
	static bool SaveVRSaveGameToSlot(UVRSaveGame* SaveGame, const FString& SlotName, const int32 UserIndex);
	
	UFUNCTION(BlueprintCallable)
	static FString GetDefaultSlotName() {return "SaveSlot01";}
	
#pragma endregion
	
};
