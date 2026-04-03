// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Core/TtAssetManager.h"
#include "Engine/Engine.h"
#include "TestTaskSolution/Core/TtGameplayTags.h"

UTtAssetManager& UTtAssetManager::Get()
{
	check(GEngine);

	UTtAssetManager* AssetManager = Cast<UTtAssetManager>(GEngine->AssetManager);
	check(AssetManager);

	return *AssetManager;
}

void UTtAssetManager::StartInitialLoading()
{
	FTtGameplayTags::InitializeNativeGameplayTags();

	Super::StartInitialLoading();
}
