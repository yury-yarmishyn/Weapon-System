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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnWeaponSlotByAmmoChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnWeaponSlotByAmmoSlotChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnWeaponSlotByWeaponDataChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTtOnAmmoSlotByAmmoDataChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTTASK_API UTtWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTtWeaponComponent();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void InitWeaponComponent(
		const TMap<ETtWeaponSlot, UTtWeaponData*>& DefaultWeaponSlotsByData,
		const TMap<ETtAmmoSlot, UTtAmmoData*>& DefaultAmmoSlotsByData);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void GrantAbilitiesFromInitData();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanEquipWeapon(ETtWeaponSlot WeaponSlot) const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanEquipAmmo(ETtAmmoSlot AmmoSlot) const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanFire(ETtAmmoSlot AmmoSlot) const;

	UFUNCTION(BlueprintPure, Category="Weapon")
	ETtWeaponSlot GetNextWeapon(ETtWeaponSlot InCurrentWeaponSlot) const;

	UFUNCTION(BlueprintPure, Category="Weapon")
	ETtWeaponSlot GetPrevWeapon(ETtWeaponSlot InCurrentWeaponSlot) const;

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
	void FireBySlot(ETtAmmoSlot AmmoSlot);

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
	const TMap<ETtWeaponSlot, int32>& GetWeaponSlotByAmmo() const { return WeaponSlotByAmmo; }
	
	UFUNCTION(BlueprintPure, Category="Weapon")
	const TMap<ETtAmmoSlot, UTtAmmoData*>& GetAmmoSlotByAmmoData() const { return AmmoSlotByAmmoData; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	const TMap<ETtWeaponSlot, ETtAmmoSlot>& GetWeaponSlotByAmmoSlot() const { return WeaponSlotByAmmoSlot; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	const TMap<ETtWeaponSlot, UTtWeaponData*>& GetWeaponSlotByWeaponData() const { return WeaponSlotByWeaponData; }
	
	// Setters
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetCurrentAmmoSlot(ETtAmmoSlot NewAmmoSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetCurrentWeaponSlot(ETtWeaponSlot NewWeaponSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetWeaponSlotByAmmo(const TMap<ETtWeaponSlot, int32>& NewWeaponSlotByAmmo);
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetAmmoSlotByAmmoData(const TMap<ETtAmmoSlot, UTtAmmoData*>& NewAmmoSlotByAmmoData);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetWeaponSlotByAmmoSlot(const TMap<ETtWeaponSlot, ETtAmmoSlot>& NewWeaponSlotByAmmoSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetAmmoSlotForWeapon(ETtWeaponSlot WeaponSlot, ETtAmmoSlot NewAmmoSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetWeaponSlotByWeaponData(const TMap<ETtWeaponSlot, UTtWeaponData*>& NewWeaponSlotByWeaponData);
	
	// Dispatchers
	
	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnAmmoSlotChanged OnAmmoSlotChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnWeaponSlotChanged OnWeaponSlotChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnWeaponSlotByAmmoChanged OnWeaponSlotByAmmoChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnAmmoSlotByAmmoDataChanged OnAmmoSlotByAmmoDataChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnWeaponSlotByAmmoSlotChanged OnWeaponSlotByAmmoSlotChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FTtOnWeaponSlotByWeaponDataChanged OnWeaponSlotByWeaponDataChanged;
	

private:
	UPROPERTY(Transient)
	ETtAmmoSlot CurrentAmmoSlot = ETtAmmoSlot::Slot1;
	
	UPROPERTY(Transient)
	ETtWeaponSlot CurrentWeaponSlot = ETtWeaponSlot::Slot1;
	
	UPROPERTY(Transient)
	TMap<ETtWeaponSlot, int32> WeaponSlotByAmmo;
	
	UPROPERTY(Transient)
	TMap<ETtAmmoSlot, UTtAmmoData*> AmmoSlotByAmmoData;

	UPROPERTY(Transient)
	TMap<ETtWeaponSlot, ETtAmmoSlot> WeaponSlotByAmmoSlot;
	
	UPROPERTY(Transient)
	TMap<ETtWeaponSlot, UTtWeaponData*> WeaponSlotByWeaponData;
	
	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> WeaponHandler = nullptr;
};
