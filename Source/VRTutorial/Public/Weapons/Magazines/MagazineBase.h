// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagazineBase.generated.h"

class AAmmoBase;
class UStaticMeshComponent;
class UAmmoContainerComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class VRTUTORIAL_API AMagazineBase : public AActor
{
	GENERATED_BODY()
	
public:	

#pragma region Variables
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CarryMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAmmoContainerComponent* AmmoContainer;
	
#pragma endregion
	
#pragma region Functions
	// Sets default values for this actor's properties
	AMagazineBase();
	
	UFUNCTION(BlueprintCallable)
	void UpdateCarryPosition();
	
	UFUNCTION(BlueprintCallable)
	virtual void RefillMag();
	
	UFUNCTION(BlueprintCallable)
	virtual bool RemoveSingleRound();
	
	UFUNCTION(BlueprintCallable)
	virtual void UpdateAllAmmoInstances();
	
	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmoCount();
	
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAmmoBase> GetAmmoClass();
	
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
