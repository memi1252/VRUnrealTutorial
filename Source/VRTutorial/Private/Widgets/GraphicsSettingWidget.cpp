// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GraphicsSettingWidget.h"

#include <Components/Button.h>
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/GameUserSettings.h>

void UGraphicsSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	switch (UGameUserSettings::GetGameUserSettings()->GetOverallScalabilityLevel())
	{
	case 0:
		{
			SetAllLow();
			break;
		}
	case 1:
		{
			SetAllMedium();
			break;
		}
	case 2:
		{
			SetAllHigh();
			break;
		}
	case 3:
		{
			SetAllEpic();
			break;
		}
	default:
		{
			SetAllEpic();
			break;
		}
	}
}

void UGraphicsSettingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	AllLowButton->OnPressed.AddDynamic(this, &UGraphicsSettingWidget::SetAllLow);
	AllMediumButton->OnPressed.AddDynamic(this, &UGraphicsSettingWidget::SetAllMedium);
	AllHighButton->OnPressed.AddDynamic(this, &UGraphicsSettingWidget::SetAllHigh);
	AllEpicButton->OnPressed.AddDynamic(this, &UGraphicsSettingWidget::SetAllEpic);
}

void UGraphicsSettingWidget::SetAllLow()
{
	UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(0);
	
	AllLowButton->SetBackgroundColor(SelectedColor);
	
	AllMediumButton->SetBackgroundColor(UnselectedColor);
	AllHighButton->SetBackgroundColor(UnselectedColor);
	AllEpicButton->SetBackgroundColor(UnselectedColor);
	
	UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
}

void UGraphicsSettingWidget::SetAllMedium()
{
	UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(1);
	
	AllMediumButton->SetBackgroundColor(SelectedColor);
	
	AllLowButton->SetBackgroundColor(UnselectedColor);
	AllHighButton->SetBackgroundColor(UnselectedColor);
	AllEpicButton->SetBackgroundColor(UnselectedColor);
	
	UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
}

void UGraphicsSettingWidget::SetAllHigh()
{
	UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(2);
	
	AllHighButton->SetBackgroundColor(SelectedColor);
	
	AllLowButton->SetBackgroundColor(UnselectedColor);
	AllMediumButton->SetBackgroundColor(UnselectedColor);
	AllEpicButton->SetBackgroundColor(UnselectedColor);
	
	UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
}

void UGraphicsSettingWidget::SetAllEpic()
{
	UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(3);
	
	AllEpicButton->SetBackgroundColor(SelectedColor);
	
	AllLowButton->SetBackgroundColor(UnselectedColor);
	AllMediumButton->SetBackgroundColor(UnselectedColor);
	AllHighButton->SetBackgroundColor(UnselectedColor);
	
	UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
}
