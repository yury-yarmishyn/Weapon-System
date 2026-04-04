// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TtProjectileData.generated.h"

UCLASS(BlueprintType)
class TESTTASK_API UTtProjectileData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<ATtProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float CollisionRadius = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float InitialSpeed = 2500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float MaxSpeed = 2500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float GravityScale = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float LifeSeconds = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	bool bShouldBounce = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	bool bDestroyOnHit = true;
};
