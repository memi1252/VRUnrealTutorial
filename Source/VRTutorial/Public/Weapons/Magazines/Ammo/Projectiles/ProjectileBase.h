// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USoundBase;
class UNiagaraSystem;
class UMaterialInstance;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class VRTUTORIAL_API AProjectileBase : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle lifetimeTH;
	
public:	
#pragma region Variables
	
#pragma endregion
	
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	
#pragma endregion
	
#pragma region Projectile
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	float Damage = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	float ProjectileSpeed = 10000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	float Lifetime = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	float DecalLifetime = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	float DecalFadeScreenSize = 0.001f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	bool bIsExplosive;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	USoundBase* ImpactSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	UNiagaraSystem* ImpactParticles;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables | Projectile")
	UMaterialInstance* ImpactDecal;
	
#pragma endregion
	
#pragma region Functions

	// Sets default values for this actor's properties
	AProjectileBase();
	
	UFUNCTION(BlueprintCallable)
	void DestroyProjectile(const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable)
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

#pragma endregion
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
