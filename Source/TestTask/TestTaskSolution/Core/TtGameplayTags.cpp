// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Core/TtGameplayTags.h"
#include "GameplayTagsManager.h"

FTtGameplayTags FTtGameplayTags::GameplayTags;

const FTtGameplayTags& FTtGameplayTags::Get()
{
	return GameplayTags;
}

void FTtGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	GameplayTags.Event_Weapon_Fire = TagsManager.AddNativeGameplayTag(
		TEXT("Event.Weapon.Fire"),
		TEXT("Gameplay event used to activate weapon fire ability."));
	
	GameplayTags.Event_Weapon_Reload = TagsManager.AddNativeGameplayTag(
    	TEXT("Event.Weapon.Reload"),
    	TEXT("Gameplay event used to activate weapon reload ability."));
	
	GameplayTags.Event_Weapon_Equip = TagsManager.AddNativeGameplayTag(
		TEXT("Event.Weapon.Equip"),
		TEXT("Gameplay event used to activate weapon equip ability."));

	GameplayTags.Event_Ammo_Equip = TagsManager.AddNativeGameplayTag(
		TEXT("Event.Ammo.Equip"),
		TEXT("Gameplay event used to activate ammo equip ability."));
	
	GameplayTags.Ammo_Regular = TagsManager.AddNativeGameplayTag(
		TEXT("Ammo.Regular"),
		TEXT("Regular ammo behavior."));
	GameplayTags.Ammo_Fire = TagsManager.AddNativeGameplayTag(
		TEXT("Ammo.Fire"),
		TEXT("Fire ammo behavior."));
	GameplayTags.Ammo_Water = TagsManager.AddNativeGameplayTag(
		TEXT("Ammo.Water"),
		TEXT("Water ammo behavior."));
	GameplayTags.Ammo_Healing = TagsManager.AddNativeGameplayTag(
		TEXT("Ammo.Healing"),
		TEXT("Healing ammo behavior."));

	GameplayTags.Status_Burning = TagsManager.AddNativeGameplayTag(
		TEXT("Status.Burning"),
		TEXT("Actor is burning and receives periodic fire damage."));

	TagsManager.DoneAddingNativeTags();
}
