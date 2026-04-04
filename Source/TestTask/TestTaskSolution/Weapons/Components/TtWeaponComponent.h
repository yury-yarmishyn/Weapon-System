// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/Data/TtAmmoData.h"
#include "Weapons/Data/TtWeaponData.h"
#include "TtWeaponComponent.generated.h"

class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTtOnAmmoSlotChanged, ETtAmmoSlot, OldAmmoSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTtOnWeaponSlotChanged, ETtWeaponSlot, OldWeaponSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnCurrentAmmoByWeaponSlotsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnActiveAmmoSlotsByWeaponSlotsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnActiveWeaponDataByWeaponSlotsChanged);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTTASK_API UTtWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTtWeaponComponent();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanEquipWeapon(const UTtWeaponData* WeaponData) const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanEquipAmmo(const UTtAmmoData* AmmoData) const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipWeapon(const UTtWeaponData* WeaponData);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipAmmo(const UTtAmmoData* AmmoData);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Shoot(const UTtAmmoData* AmmoData);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Reload(const ETtWeaponSlot InWeaponSlot);
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ReloadAll();
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DecreaseAmmoInSlot(ETtWeaponSlot InWeaponSlot, int32 InValue);
	
	// Getters
	
	UFUNCTION(BlueprintPure, Category="Weapon")
	ETtAmmoSlot GetCurrentAmmoSlot() const { return CurrentAmmoSlot; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	ETtWeaponSlot GetCurrentWeaponSlot() const { return CurrentWeaponSlot; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	const TMap<ETtWeaponSlot, int32>& GetCurrentAmmoByWeaponSlots() const { return CurrentAmmoByWeaponSlots; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	const TMap<ETtWeaponSlot, ETtAmmoSlot>& GetActiveAmmoSlotsByWeaponSlots() const { return ActiveAmmoSlotsByWeaponSlots; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	const TMap<ETtWeaponSlot, UTtWeaponData*>& GetActiveWeaponDataByWeaponSlots() const { return ActiveWeaponDataByWeaponSlots; }
	
	// Setters
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetCurrentAmmoSlot(ETtAmmoSlot NewAmmoSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetCurrentWeaponSlot(ETtWeaponSlot NewWeaponSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetCurrentAmmoByWeaponSlots(const TMap<ETtWeaponSlot, int32>& NewCurrentAmmoByWeaponSlots);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetActiveAmmoSlotsByWeaponSlots(const TMap<ETtWeaponSlot, ETtAmmoSlot>& NewActiveAmmoSlotsByWeaponSlots);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetActiveWeaponDataByWeaponSlots(const TMap<ETtWeaponSlot, UTtWeaponData*>& NewActiveWeaponDataByWeaponSlots);
	
	// Dispatchers
	
	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnAmmoSlotChanged OnAmmoSlotChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnWeaponSlotChanged OnWeaponSlotChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnCurrentAmmoByWeaponSlotsChanged OnCurrentAmmoByWeaponSlotsChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnActiveAmmoSlotsByWeaponSlotsChanged OnActiveAmmoSlotsByWeaponSlotsChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnActiveWeaponDataByWeaponSlotsChanged OnActiveWeaponDataByWeaponSlotsChanged;
	

private:
	UPROPERTY(Transient, Category="Weapon")
	ETtAmmoSlot CurrentAmmoSlot = ETtAmmoSlot::Slot1;
	
	UPROPERTY(Transient, Category="Weapon")
	ETtWeaponSlot CurrentWeaponSlot = ETtWeaponSlot::Slot1;
	
	UPROPERTY(Transient, Category="Weapon")
	TMap<ETtWeaponSlot, int32> CurrentAmmoByWeaponSlots;

	UPROPERTY(Transient, Category="Weapon")
	TMap<ETtWeaponSlot, ETtAmmoSlot> ActiveAmmoSlotsByWeaponSlots;
	
	UPROPERTY(Transient, Category="Weapon")
	TMap<ETtWeaponSlot, UTtWeaponData*> ActiveWeaponDataByWeaponSlots;
	
	UPROPERTY(Transient, Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponHandler = nullptr;
};
