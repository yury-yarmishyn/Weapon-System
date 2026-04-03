// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TtProjectile.generated.h"

class UPrimitiveComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UTtProjectileData;

UCLASS()
class TESTTASK_API ATtProjectile : public AActor
{
	GENERATED_BODY()

public:
	ATtProjectile();

protected:
	struct FTtProjectileImpactContext
	{
		AActor* ProjectileOwner = nullptr;
		AActor* ProjectileInstigator = nullptr;
		AActor* HitActor = nullptr;
		FHitResult HitResult;
	};

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleCollisionHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	void BuildImpactContext(AActor* HitActor, const FHitResult& HitResult, FTtProjectileImpactContext& OutContext) const;
	virtual void ForwardImpactToEffectPipeline(const FTtProjectileImpactContext& ImpactContext);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TObjectPtr<UTtProjectileData> ProjectileData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float LifeSeconds = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	bool bDestroyOnImpact = true;
};
