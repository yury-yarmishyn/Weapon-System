// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TtWeaponTypes.generated.h"

class ATtProjectile;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ETtAmmoSlot : uint8
{
	Slot1 UMETA(DisplayName = "Slot 1"),
	Slot2 UMETA(DisplayName = "Slot 2"),
	Slot3 UMETA(DisplayName = "Slot 3"),
	Slot4 UMETA(DisplayName = "Slot 4")
};

USTRUCT(BlueprintType)
struct FTtAmmoDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	FName AmmoId = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	FGameplayTag AmmoTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	TSubclassOf<ATtProjectile> ProjectileClassOverride;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	TSubclassOf<UGameplayAbility> EquipAmmoAbilityClass;
};

class TESTTASK_API FTtWeaponTypes final
{
public:
	FTtWeaponTypes();
};
