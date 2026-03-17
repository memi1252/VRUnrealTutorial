// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphicsSettingWidget.generated.h"

class UButton;

UCLASS()
class VRTUTORIAL_API UGraphicsSettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
#pragma region Variables
	
	UPROPERTY(meta = (BindWidget))
	UButton* AllLowButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* AllMediumButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* AllHighButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* AllEpicButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	FLinearColor SelectedColor = FLinearColor::Blue;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	FLinearColor UnselectedColor = FLinearColor::Gray;	
	
#pragma endregion
	
#pragma region Functions
	
	virtual void NativeConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void SetAllLow();
	
	UFUNCTION(BlueprintCallable)
	void SetAllMedium();

	UFUNCTION(BlueprintCallable)
	void SetAllHigh();
	
	UFUNCTION(BlueprintCallable)
	void SetAllEpic();
	
#pragma endregion
};
