// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBase.generated.h"

class AProjectileBase;
class UStaticMeshComponent;

UCLASS()
class VRTUTORIAL_API AAmmoBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
#pragma region Variables

#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* AmmoMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FiredAmmoMesh;
	
#pragma endregion
	
#pragma region Ammo
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Ammo")
	TSubclassOf<AProjectileBase> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Ammo")
	float DespawnDelay = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Ammo")
	bool bWasFired;
	
#pragma endregion
	
#pragma endregion
	
#pragma region Functions
	// Sets default values for this actor's properties
	AAmmoBase();
	
	void InitDespawnAmmo();
	
	void DespawnAmmo();
	
	void SetAmmoFired();
	
#pragma endregion
};
