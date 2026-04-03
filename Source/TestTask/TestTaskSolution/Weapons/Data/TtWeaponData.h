// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Weapons/Data/TtWeaponTypes.h"
#include "TtWeaponData.generated.h"

class ATtProjectile;
class UAnimMontage;
class UGameplayAbility;
class USkeletalMesh;
class UTtProjectileData;

/**
 * Data-driven weapon setup.
 */
UCLASS(BlueprintType)
class TESTTASK_API UTtWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName WeaponId = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	FGameplayTag EquipEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	FGameplayTag FireEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	FGameplayTag ReloadEventTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Projectile")
	TObjectPtr<UTtProjectileData> ProjectileData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Projectile")
	TSubclassOf<ATtProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	TArray<FTtAmmoDefinition> AmmoDefinitions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	TMap<ETtAmmoSlot, FName> SlotAmmoMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	int32 MagazineSize = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammo")
	bool bInfiniteAmmo = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Stats")
	float FireRate = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Stats")
	float ReloadDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	TSubclassOf<UGameplayAbility> FireAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	TSubclassOf<UGameplayAbility> ReloadAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Abilities")
	TSubclassOf<UGameplayAbility> EquipAmmoAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Visuals")
	TObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Animations")
	TObjectPtr<UAnimMontage> ReloadMontage;
};
