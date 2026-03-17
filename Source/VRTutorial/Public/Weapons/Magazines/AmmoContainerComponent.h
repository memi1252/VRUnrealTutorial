// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoContainerComponent.generated.h"

class AAmmoBase;
class UStaticMeshComponent;
class UInstancedStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTUTORIAL_API UAmmoContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	#pragma region Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Ammo Container")
	TSubclassOf<AAmmoBase> AmmoClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Ammo Container")
	int CurrentAmmoCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Ammo Container")
	int MaxAmmoCount = 10;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Ammo Container")
	UStaticMeshComponent* MeshRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | Ammo Container")
	UInstancedStaticMeshComponent* StoredAmmo;
	
	#pragma endregion
	
#pragma region Variables
	
	// Sets default values for this component's properties
	UAmmoContainerComponent();
	
	UFUNCTION(BlueprintCallable)
	void RefillAmmo();
	
	UFUNCTION(BlueprintCallable)
	void UpdateAllAmmoInstances();
	
	UFUNCTION(BlueprintCallable)
	bool RemoveSingleRound();
	
	UFUNCTION(BlueprintCallable)
	bool AddSingleRound();

#pragma endregion
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

		
};
