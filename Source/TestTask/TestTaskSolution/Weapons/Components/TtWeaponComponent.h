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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnAmmoByWeaponSlotsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnActiveAmmoSlotsByWeaponSlotsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnActiveWeaponDataByWeaponSlotsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnAmmoDataBySlotChanged);

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
	void EquipWeaponBySlot(const ETtWeaponSlot InWeaponSlot);
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipWeaponByData(const UTtWeaponData* WeaponData);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipAmmoBySlot(const ETtAmmoSlot InAmmoSlot);
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipAmmoByData(const UTtAmmoData* AmmoData);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire(const UTtAmmoData* AmmoData);

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
	const TMap<ETtWeaponSlot, int32>& GetAmmoByWeaponSlots() const { return AmmoByWeaponSlots; }
	
	UFUNCTION(BlueprintPure, Category="Weapon")
	const TMap<ETtAmmoSlot, UTtAmmoData*>& GetAmmoDataBySlot() const { return AmmoDataBySlot; }

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
	void SetAmmoByWeaponSlots(const TMap<ETtWeaponSlot, int32>& NewAmmoByWeaponSlots);
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetAmmoDataBySlot(const TMap<ETtAmmoSlot, UTtAmmoData*>& NewAmmoDataBySlot);

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
	FTtOnAmmoByWeaponSlotsChanged OnAmmoByWeaponSlotsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnAmmoDataBySlotChanged OnAmmoDataBySlotChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnActiveAmmoSlotsByWeaponSlotsChanged OnActiveAmmoSlotsByWeaponSlotsChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnActiveWeaponDataByWeaponSlotsChanged OnActiveWeaponDataByWeaponSlotsChanged;
	

private:
	UPROPERTY(Transient)
	ETtAmmoSlot CurrentAmmoSlot = ETtAmmoSlot::Slot1;
	
	UPROPERTY(Transient)
	ETtWeaponSlot CurrentWeaponSlot = ETtWeaponSlot::Slot1;
	
	UPROPERTY(Transient)
	TMap<ETtWeaponSlot, int32> AmmoByWeaponSlots;
	
	UPROPERTY(Transient)
	TMap<ETtAmmoSlot, UTtAmmoData*> AmmoDataBySlot;

	UPROPERTY(Transient)
	TMap<ETtWeaponSlot, ETtAmmoSlot> ActiveAmmoSlotsByWeaponSlots;
	
	UPROPERTY(Transient)
	TMap<ETtWeaponSlot, UTtWeaponData*> ActiveWeaponDataByWeaponSlots;
	
	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> WeaponHandler = nullptr;
};
