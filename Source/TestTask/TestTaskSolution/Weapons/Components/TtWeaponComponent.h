// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TtWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class ETtAmmoSlot : uint8
{
	Slot1 UMETA(DisplayName = "Slot 1"),
	Slot2 UMETA(DisplayName = "Slot 2"),
	Slot3 UMETA(DisplayName = "Slot 3"),
	Slot4 UMETA(DisplayName = "Slot 4")
};

/**
 * Gameplay-only weapon orchestrator.
 */
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

