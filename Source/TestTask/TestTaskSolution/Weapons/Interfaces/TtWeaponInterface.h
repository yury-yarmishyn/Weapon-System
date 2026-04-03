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
	virtual USkeletalMeshComponent* GetWeaponHandler() const = 0;
	virtual UTtWeaponComponent* GetWeaponComponent() const = 0;
};

