// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtFireProjectileAbility.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "TestTask.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Data/TtAmmoData.h"
#include "TestTaskSolution/Weapons/Data/TtProjectileData.h"
#include "TestTaskSolution/Weapons/Interfaces/TtWeaponInterface.h"
#include "TestTaskSolution/Weapons/Projectiles/TtProjectile.h"

UTtFireProjectileAbility::UTtFireProjectileAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTtFireProjectileAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	UTtWeaponComponent* WeaponComponent = GetWeaponComponentFromActorInfo(ActorInfo);
	if (!AvatarActor || !WeaponComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const ETtWeaponSlot CurrentWeaponSlot = WeaponComponent->GetCurrentWeaponSlot();
	const int32* CurrentAmmoPtr = WeaponComponent->GetWeaponSlotByAmmo().Find(CurrentWeaponSlot);
	if (!CurrentAmmoPtr || *CurrentAmmoPtr <= 0)
	{
		UE_LOG(LogTestTask, Warning, TEXT("Fire ability failed: no ammo in current weapon slot"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const UTtAmmoData* AmmoData = TriggerEventData ? Cast<UTtAmmoData>(TriggerEventData->OptionalObject) : nullptr;
	if (!AmmoData)
	{
		if (UTtAmmoData* const* FoundAmmo = WeaponComponent->GetAmmoSlotByAmmoData().Find(WeaponComponent->GetCurrentAmmoSlot()))
		{
			AmmoData = *FoundAmmo;
		}
	}

	if (!AmmoData || !AmmoData->ProjectileData || !AmmoData->ProjectileData->ProjectileClass || !AvatarActor->GetWorld())
	{
		UE_LOG(LogTestTask, Warning, TEXT("Fire ability failed: invalid ammo or projectile data"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector SpawnLocation = AvatarActor->GetActorLocation() + AvatarActor->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = AvatarActor->GetActorRotation();

	if (const ITtWeaponInterface* WeaponInterface = Cast<ITtWeaponInterface>(AvatarActor))
	{
		if (const USkeletalMeshComponent* WeaponHandler = WeaponInterface->GetWeaponHandler())
		{
			SpawnLocation = WeaponHandler->GetComponentLocation();
			SpawnRotation = WeaponHandler->GetComponentRotation();
		}
	}

	const FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	ATtProjectile* SpawnedProjectile = AvatarActor->GetWorld()->SpawnActorDeferred<ATtProjectile>(
		AmmoData->ProjectileData->ProjectileClass,
		SpawnTransform,
		AvatarActor,
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!SpawnedProjectile)
	{
		UE_LOG(LogTestTask, Warning, TEXT("Fire ability failed: projectile spawn failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	SpawnedProjectile->ProjectileData = AmmoData->ProjectileData;
	SpawnedProjectile->FinishSpawning(SpawnTransform);

	UE_LOG(LogTestTask, Log, TEXT("Fire animation placeholder for %s"), *GetNameSafe(AvatarActor));

	WeaponComponent->DecreaseAmmoInSlot(CurrentWeaponSlot, 1);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
