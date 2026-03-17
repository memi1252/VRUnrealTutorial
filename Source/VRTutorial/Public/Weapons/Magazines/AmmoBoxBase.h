// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBoxBase.generated.h"

class AAmmoBase;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UAmmoContainerComponent;

UCLASS()
class VRTUTORIAL_API AAmmoBoxBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
#pragma region Variables
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AAmmoBase* SpawnedAmmo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAmmoContainerComponent* AmmoContainer;
	
#pragma endregion
	
#pragma region Functions
	
	// Sets default values for this actor's properties
	AAmmoBoxBase();
	
	UFUNCTION(BlueprintCallable)
	AActor* TakeAmmo(USkeletalMeshComponent* CharacterMesh, const FName& BoneToAttachTo);
	
	UFUNCTION(BlueprintCallable)
	void ReturnAmmo();
	
	UFUNCTION(BlueprintCallable)
	void ClearSpawnedAmmo();
	
#pragma endregion

};
