// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Core/TtGameplayTags.h"

namespace TtGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Event_Weapon_Fire,
		"Event.Weapon.Fire",
		"Gameplay event used to activate weapon fire ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Event_Weapon_Reload,
		"Event.Weapon.Reload",
		"Gameplay event used to activate weapon reload ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Event_Weapon_Equip,
		"Event.Weapon.Equip",
		"Gameplay event used to activate weapon equip ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Event_Ammo_Equip,
		"Event.Ammo.Equip",
		"Gameplay event used to activate ammo equip ability.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Ammo_Regular, 
		"Ammo.Regular", 
		"Regular ammo behavior.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Ammo_Fire, 
		"Ammo.Fire", 
		"Fire ammo behavior.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Ammo_Water,
		"Ammo.Water",
		"Water ammo behavior.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Ammo_Healing,
		"Ammo.Healing",
		"Healing ammo behavior.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Status_Burning,
		"Status.Burning",
		"Actor is burning and receives periodic fire damage.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Data_Damage_Physical,
		"Data.Damage.Physical",
		"SetByCaller data tag for physical damage magnitude.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Data_Damage_Fire,
		"Data.Damage.Fire",
		"SetByCaller data tag for fire damage magnitude.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Data_Damage_Water,
		"Data.Damage.Water",
		"SetByCaller data tag for water damage magnitude.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		TAG_Data_Healing,
		"Data.Healing",
		"SetByCaller data tag for healing magnitude.");
}
