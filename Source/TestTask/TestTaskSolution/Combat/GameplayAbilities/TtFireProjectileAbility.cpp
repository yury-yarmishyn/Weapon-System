// Fill out your copyright notice in the Description page of Project Settings.

#include "TestTaskSolution/Combat/GameplayAbilities/TtFireProjectileAbility.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "TestTask.h"
#include "Core/TtGameplayTags.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"
#include "TestTaskSolution/Weapons/Data/TtAmmoData.h"
#include "TestTaskSolution/Weapons/Data/TtProjectileData.h"
#include "TestTaskSolution/Weapons/Data/TtWeaponData.h"
#include "TestTaskSolution/Weapons/Interfaces/TtWeaponInterface.h"
#include "TestTaskSolution/Weapons/Projectiles/TtProjectile.h"

UTtFireProjectileAbility::UTtFireProjectileAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	const FGameplayTag TriggerTag = TtGameplayTags::TAG_Event_Weapon_Fire;

	if (TriggerTag.IsValid())
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		TriggerData.TriggerTag = TriggerTag;
		AbilityTriggers.Add(TriggerData);
	}
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
		UE_LOG(LogTestTask, Warning, TEXT("Fire ability activation rejected: missing avatar or weapon component"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const ETtWeaponSlot CurrentWeaponSlot = WeaponComponent->GetCurrentWeaponSlot();
	ETtAmmoSlot FireAmmoSlot = WeaponComponent->GetCurrentAmmoSlot();
	if (TriggerEventData)
	{
		const UTtAmmoData* AmmoData = Cast<UTtAmmoData>(TriggerEventData->OptionalObject);
		if (AmmoData)
		{
			FireAmmoSlot = FindAmmoSlotByData(WeaponComponent, AmmoData, FireAmmoSlot);
		}
	}

	const UTtAmmoData* AmmoData = TriggerEventData ? Cast<UTtAmmoData>(TriggerEventData->OptionalObject) : nullptr;
	if (!AmmoData)
	{
		const UTtAmmoData* const* AmmoDataPtr = WeaponComponent->GetAmmoSlotByAmmoData().Find(FireAmmoSlot);
		AmmoData = AmmoDataPtr ? *AmmoDataPtr : nullptr;
	}

	UWorld* World = AvatarActor->GetWorld();
	if (!AmmoData || !AmmoData->ProjectileData || !AmmoData->ProjectileData->ProjectileClass || !World)
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("[%s] Fire ability activation rejected: invalid ammo/projectile data"),
			*GetNameSafe(AvatarActor));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector SpawnLocation = AvatarActor->GetActorLocation() + AvatarActor->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = AvatarActor->GetActorRotation();

	if (AvatarActor->GetClass()->ImplementsInterface(UTtWeaponInterface::StaticClass()))
	{
		if (const USkeletalMeshComponent* WeaponMesh = ITtWeaponInterface::Execute_GetWeaponMesh(AvatarActor))
		{
			FName MuzzleSocketName = TEXT("Muzzle");
			if (const UTtWeaponData* const* WeaponDataPtr = WeaponComponent->GetWeaponSlotByWeaponData().Find(CurrentWeaponSlot))
			{
				if (const UTtWeaponData* WeaponData = *WeaponDataPtr)
				{
					MuzzleSocketName = WeaponData->MuzzleSocketName;
				}
			}

			if (WeaponMesh->DoesSocketExist(MuzzleSocketName))
			{
				SpawnLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
				SpawnRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
			}
			else
			{
				SpawnLocation = WeaponMesh->GetComponentLocation();
				SpawnRotation = WeaponMesh->GetComponentRotation();
			}
		}
		else if (const USkeletalMeshComponent* WeaponHandler = ITtWeaponInterface::Execute_GetWeaponHandler(AvatarActor))
		{
			SpawnLocation = WeaponHandler->GetComponentLocation();
			SpawnRotation = WeaponHandler->GetComponentRotation();
		}
	}

	const FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	ATtProjectile* SpawnedProjectile = World->SpawnActorDeferred<ATtProjectile>(
		AmmoData->ProjectileData->ProjectileClass,
		SpawnTransform,
		AvatarActor,
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!SpawnedProjectile)
	{
		UE_LOG(LogTestTask, Warning, TEXT("[%s] Failed to spawn projectile"), *GetNameSafe(AvatarActor));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	SpawnedProjectile->ProjectileData = AmmoData->ProjectileData;
	SpawnedProjectile->FinishSpawning(SpawnTransform);

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Fire executed. WeaponSlot=%d AmmoSlot=%d Projectile=%s"),
		*GetNameSafe(AvatarActor),
		static_cast<int32>(CurrentWeaponSlot),
		static_cast<int32>(FireAmmoSlot),
		*GetNameSafe(SpawnedProjectile));

	PendingWeaponComponent = WeaponComponent;
	PendingWeaponSlot = CurrentWeaponSlot;

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		if (IsValid(SpawnedProjectile))
		{
			SpawnedProjectile->Destroy();
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UTtFireProjectileAbility::CommitAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	UTtWeaponComponent* WeaponComponent = PendingWeaponComponent.Get();
	if (!WeaponComponent)
	{
		return false;
	}

	if (!Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}

	WeaponComponent->DecreaseAmmoInSlot(PendingWeaponSlot, 1);

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Fire ability commit complete. WeaponSlot=%d"),
		*GetNameSafe(WeaponComponent->GetOwner()),
		static_cast<int32>(PendingWeaponSlot));

	return true;
}
