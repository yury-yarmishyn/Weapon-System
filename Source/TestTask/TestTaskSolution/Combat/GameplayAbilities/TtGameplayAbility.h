// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Weapons/Data/TtAmmoData.h"
#include "Weapons/Data/TtWeaponData.h"
#include "TtGameplayAbility.generated.h"

class UTtAmmoData;
class UTtWeaponComponent;
class UTtWeaponData;

/**
 * 
 */
UCLASS()
class TESTTASK_API UTtGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UTtWeaponComponent* GetWeaponComponentFromActorInfo(const FGameplayAbilityActorInfo* ActorInfo) const;
	ETtWeaponSlot FindWeaponSlotByData(
		const UTtWeaponComponent* WeaponComponent,
		const UTtWeaponData* WeaponData,
		ETtWeaponSlot FallbackSlot) const;
	ETtAmmoSlot FindAmmoSlotByData(
		const UTtWeaponComponent* WeaponComponent,
		const UTtAmmoData* AmmoData,
		ETtAmmoSlot FallbackSlot) const;
};
