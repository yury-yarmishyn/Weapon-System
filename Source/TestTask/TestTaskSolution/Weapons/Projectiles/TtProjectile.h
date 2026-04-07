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

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void InitProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile", meta=(ExposeOnSpawn="true"))
	TObjectPtr<UTtProjectileData> ProjectileData = nullptr;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCollisionHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

private:
	void ApplyOnHitEffect(AActor* HitActor) const;

private:
	UPROPERTY(VisibleAnywhere, Category="Projectile|Components")
	TObjectPtr<USphereComponent> CollisionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Projectile|Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement = nullptr;
};
