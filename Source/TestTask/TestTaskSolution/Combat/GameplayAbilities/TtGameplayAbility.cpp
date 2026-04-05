// Fill out your copyright notice in the Description page of Project Settings.

#include "TtGameplayAbility.h"

#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Interfaces/TtWeaponInterface.h"

UTtWeaponComponent* UTtGameplayAbility::GetWeaponComponentFromActorInfo(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (!ActorInfo)
	{
		return nullptr;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!AvatarActor)
	{
		AvatarActor = ActorInfo->OwnerActor.Get();
	}

	if (!AvatarActor)
	{
		return nullptr;
	}

	if (const ITtWeaponInterface* WeaponInterface = Cast<ITtWeaponInterface>(AvatarActor))
	{
		return WeaponInterface->GetWeaponComponent();
	}

	return nullptr;
}

ETtWeaponSlot UTtGameplayAbility::FindWeaponSlotByData(
	const UTtWeaponComponent* WeaponComponent,
	const UTtWeaponData* WeaponData,
	const ETtWeaponSlot FallbackSlot) const
{
	if (!WeaponComponent || !WeaponData)
	{
		return FallbackSlot;
	}

	for (const TPair<ETtWeaponSlot, UTtWeaponData*>& Pair : WeaponComponent->GetWeaponSlotByWeaponData())
	{
		if (Pair.Value == WeaponData)
		{
			return Pair.Key;
		}
	}

	return FallbackSlot;
}

ETtAmmoSlot UTtGameplayAbility::FindAmmoSlotByData(
	const UTtWeaponComponent* WeaponComponent,
	const UTtAmmoData* AmmoData,
	const ETtAmmoSlot FallbackSlot) const
{
	if (!WeaponComponent || !AmmoData)
	{
		return FallbackSlot;
	}

	for (const TPair<ETtAmmoSlot, UTtAmmoData*>& Pair : WeaponComponent->GetAmmoSlotByAmmoData())
	{
		if (Pair.Value == AmmoData)
		{
			return Pair.Key;
		}
	}

	return FallbackSlot;
}
