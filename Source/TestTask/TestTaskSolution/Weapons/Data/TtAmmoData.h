// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TtAmmoData.generated.h"

class UTtProjectileData;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ETtAmmoSlot : uint8
{
	Slot1 UMETA(DisplayName = "Slot 1"),
	Slot2 UMETA(DisplayName = "Slot 2"),
	Slot3 UMETA(DisplayName = "Slot 3"),
	Slot4 UMETA(DisplayName = "Slot 4")
};

/**
 * 
 */
UCLASS()
class TESTTASK_API UTtAmmoData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	FGameplayTag AmmoTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	TSubclassOf<UGameplayAbility> EquipAmmoAbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Projectile")
	TObjectPtr<UTtProjectileData> ProjectileData;
	
	// Can be extended with dedicated hit-scan data in future without changing projectile flow.
};
