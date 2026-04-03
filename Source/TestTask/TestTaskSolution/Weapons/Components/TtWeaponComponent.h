// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/Data/TtWeaponTypes.h"
#include "TtWeaponComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTTASK_API UTtWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTtWeaponComponent();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SelectAmmoSlot(ETtAmmoSlot NewSlot);

	UFUNCTION(BlueprintPure, Category="Weapon")
	ETtAmmoSlot GetCurrentAmmoSlot() const { return CurrentAmmoSlot; }

private:
	UPROPERTY(EditAnywhere, Category="Weapon")
	ETtAmmoSlot CurrentAmmoSlot = ETtAmmoSlot::Slot1;
};
