// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TtAmmoData.h"
#include "TtWeaponData.generated.h"


class ATtProjectile;
class UAnimInstance;
class UAnimMontage;
class UGameplayAbility;
class USkeletalMesh;
class UTtProjectileData;


UENUM(BlueprintType)
enum class ETtWeaponSlot : uint8
{
	Slot1 UMETA(DisplayName = "Slot 1"),
	Slot2 UMETA(DisplayName = "Slot 2"),
	Slot3 UMETA(DisplayName = "Slot 3"),
	Slot4 UMETA(DisplayName = "Slot 4")
};

/**
 * Data-driven weapon setup.
 */
UCLASS(BlueprintType)
class TESTTASK_API UTtWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	int32 MagazineSize = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	TSubclassOf<UGameplayAbility> FireAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	TSubclassOf<UGameplayAbility> ReloadAbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	TSubclassOf<UGameplayAbility> EquipAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Visuals")
	TObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Visuals")
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Visuals")
	FName MuzzleSocketName = TEXT("Muzzle");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TObjectPtr<UAnimMontage> ReloadMontage;
};
