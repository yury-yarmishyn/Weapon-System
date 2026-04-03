// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TtAssetManager.generated.h"

/**
 * Project asset manager entry point for early runtime initialization.
 */
UCLASS()
class TESTTASK_API UTtAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UTtAssetManager& Get();

	virtual void StartInitialLoading() override;
};
