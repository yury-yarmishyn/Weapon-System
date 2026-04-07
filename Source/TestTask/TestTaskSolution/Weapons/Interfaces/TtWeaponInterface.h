// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TtWeaponInterface.generated.h"

class USkeletalMeshComponent;
class UTtWeaponComponent;

UINTERFACE(BlueprintType)
class TESTTASK_API UTtWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Decouples weapon users from concrete owner classes.
 */
class TESTTASK_API ITtWeaponInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapon")
	USkeletalMeshComponent* GetWeaponHandler() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapon")
	UTtWeaponComponent* GetWeaponComponent() const;
};
