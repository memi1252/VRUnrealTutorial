// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponEnumTypes.generated.h"

UENUM(BlueprintType)
enum EWeaponMeleeType
{
	WMT_Blunt UMETA(DisplayName = "Blunt"),
	WMT_Sharp UMETA(DisplayName = "Sharp")
};

UENUM(BlueprintType)
enum EFiringMode
{
	FM_Single UMETA(DisplayName = "Single"),
	FM_SemiAuto UMETA(DisplayName = "SemiAuto"),
	FM_Tripple UMETA(DisplayName = "Tripple"),
	FM_FullAuto UMETA(DisplayName = "FullAuto")
};

UENUM(BlueprintType)
enum EDamageType
{
	DT_None UMETA(DisplayName = "None"),
	DT_Hit UMETA(DisplayName = "Hit"),
	DT_Penetration UMETA(DisplayName = "Penetration"),
	DT_Projectile UMETA(DisplayName = "Projectile")
};

UENUM(BlueprintType)
enum EMagCheckReturnType
{
	MCRT_NotAttached,
	MCRT_NotEmpty,
	MCRT_Empty
};

UENUM(BlueprintType)
enum EAmmoCheckReturnType
{
	ACRT_Success,
	ACRT_Failed,
	ACRT_MagEmpty
};
