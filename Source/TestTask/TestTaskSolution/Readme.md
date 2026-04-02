# TestTaskSolution Specification

## 1. Purpose

This document is the working project specification for `Source/TestTask/TestTaskSolution/`.
It defines the target architecture, key runtime entities, and implementation rules for the test task.

Core formula:

`GAS-first + EventDriven + ComponentOriented + DataDriven`

---

## 2. Task Scope

Required gameplay:
- ranged weapon shooting with projectile logic;
- 4 ammo behaviors:
  - `Regular` deals normal damage;
  - `Fire` deals fire damage and applies `On Fire`;
  - `Water` removes fire effects;
  - `Healing` restores health;
- `On Fire` deals periodic damage, expires over time, and can be removed by water;
- damage types and resistances;
- health display above actors.

Quality goals from the test task:
- encapsulation;
- extensibility;
- clarity;
- useful logs;
- core logic in `C++`, Blueprint only as a support layer.

---

## 3. Core Decisions

### 3.1. GAS

`Gameplay Ability System` is the project foundation.

It is used for:
- abilities;
- attributes;
- damage and healing;
- status effects;
- tags;
- gameplay events.

### 3.2. ASC Placement

`AbilitySystemComponent` lives on `TtPlayerState`, not on `TtCharacter`.

Reason:
- `PlayerState` survives beyond a specific pawn instance;
- this keeps attributes, active effects, cooldowns, and other persistent gameplay state stable across respawn or pawn replacement.

Rule:
- persistent gameplay state belongs to `PlayerState + ASC`;
- world representation belongs to `Character`.

### 3.3. Character Weapon Setup

`TtCharacter` owns two different weapon-related things:

1. `WeaponHandler`
   - not a custom class;
   - just a plain `USkeletalMeshComponent`;
   - represents arms or another weapon carrier;
   - used only for visuals, sockets, and gun attachment.

2. `TtWeaponComponent`
   - regular `UActorComponent`;
   - handles weapon gameplay orchestration.

This split is mandatory:
- `WeaponHandler` is visual only;
- `TtWeaponComponent` is gameplay only.

### 3.4. Communication Model

The system should prefer event-driven communication.

Main mechanisms:
- `SendGameplayEvent`;
- `AbilityTriggers`;
- gameplay tags;
- interfaces;
- delegates and attribute change callbacks for UI.

Key rule:
- gameplay tags are the primary source of activation validation;
- data checks still exist, but tags are the main gate for whether an action is allowed.

---

## 4. Runtime Entities

### 4.1. `TtCharacter : ACharacter`

Responsibilities:
- player pawn;
- input entry point;
- owns `WeaponHandler`;
- owns `TtWeaponComponent`;
- provides scene transforms for weapon visuals.

### 4.2. `TtPlayerState`

Responsibilities:
- owns `AbilitySystemComponent`;
- owns main `AttributeSet`;
- applies initial attributes through `GameplayEffect`;
- stores long-lived gameplay state.

### 4.3. `UTtWeaponInterface` / `ITtWeaponInterface`

Interface contracts:
- `GetWeaponHandler()`
- `GetWeaponComponent()`

Purpose:
- decouple weapons, abilities, and UI from a concrete owner class.

### 4.4. `TtWeaponComponent : UActorComponent`

Responsibilities:
- track equipped weapon data;
- track selected ammo slot;
- resolve the ammo definition assigned to the active slot;
- decide which ammo ability should be executed;
- route requests to abilities;
- expose current weapon and ammo state to other systems.

Suggested state:
- `ETtAmmoSlot CurrentAmmoSlot`

Non-responsibilities:
- no visual attachment logic;
- no direct mesh ownership;
- no heavy combat calculations.

### 4.5. `UTtWeaponDataAsset`

Weapon data asset should define:
- weapon id and display name;
- weapon tags;
- projectile data/class;
- ammo definitions;
- slot-to-ammo assignment data;
- magazine size;
- ammo rules;
- fire rate;
- reload duration;
- ability classes for fire, reload, and ammo equip;
- optional visuals and audio references.

### 4.6. `ATtProjectile` / `UTtProjectileDataAsset`

Projectile responsibilities:
- movement;
- collision;
- hit context;
- forwarding impact into the effect pipeline.

---

## 5. Ammo Model

### 5.1. Slots

Enum is used only for user-facing ammo slots.

Suggested enum:
- `ETtAmmoSlot`
  - `Slot1`
  - `Slot2`
  - `Slot3`
  - `Slot4`

Meaning:
- `1`, `2`, `3`, `4` select slots;
- slot is only a selector;
- slot must not encode the real ammo gameplay meaning.

### 5.2. Ammo Identity

Real ammo identity is data-driven.

Use:
- `DataAsset` for ammo definition;
- gameplay tags for gameplay identity.

Example tags:
- `Ammo.Regular`
- `Ammo.Fire`
- `Ammo.Water`
- `Ammo.Healing`

Rule:
- slot selects;
- data asset defines;
- gameplay tags identify behavior.

### 5.3. Equip Flow

We equip ammo, not the slot.

Flow:
1. `TtWeaponComponent` stores the active slot.
2. It resolves which ammo definition is assigned to that slot.
3. It executes `GA_Weapon_EquipAmmo` or a derived class.

Important:
- `GA_Weapon_EquipAmmo` must not depend on slot semantics;
- if a weapon needs special ammo-equip gameplay, create a subclass of `GA_Weapon_EquipAmmo`.

---

## 6. Input and Abilities

### 6.1. Input

Required bindings:
- `LMB` for fire;
- `1`, `2`, `3`, `4` for ammo slot selection.

Reload:
- exists as a dedicated ability;
- exact key can remain configurable unless explicitly required.

### 6.2. Ability Activation

Weapon abilities are triggered through:
- `SendGameplayEvent`;
- `AbilityTriggers`.

Minimum ability set:
- `GA_Weapon_Fire`
- `GA_Weapon_Reload`
- `GA_Weapon_EquipAmmo`

### 6.3. Activation Validation

Primary validation source:
- gameplay tags.

Secondary validation:
- active weapon existence;
- ammo availability;
- selected slot validity;
- current weapon state.

---

## 7. Combat Flow

### 7.1. Fire Sequence

1. Player presses `LMB`.
2. Input sends `SendGameplayEvent`.
3. `AbilityTriggers` activate `GA_Weapon_Fire`.
4. `GA_Weapon_Fire` reads weapon state from `TtWeaponComponent`.
5. Tags validate whether fire is allowed.
6. `TtWeaponComponent` resolves ammo from the active slot.
7. If needed, `GA_Weapon_EquipAmmo` is executed.
8. Projectile is spawned using weapon data and ammo definition.
9. On hit, projectile forwards context to effects.
10. Damage, healing, or status effects are applied.
11. UI updates through events, delegates, or attribute changes.

### 7.2. Damage Semantics

- `Regular` -> physical damage
- `Fire` -> fire damage + `On Fire`
- `Water` -> removes burning, may also be treated as water damage
- `Healing` -> restores `Health`

---

## 8. Attributes and Effects

### 8.1. `AttributeSet`

Minimum attributes:
- `Health`
- `MaxHealth`
- `PhysicalResistance`
- `FireResistance`
- `WaterResistance`
- `Damage`

`Damage` is a transient/meta attribute for resolving incoming damage before applying it to `Health`.

### 8.2. Initialization

Initial attributes are applied through `GameplayEffect`.

### 8.3. Status Effects

Status effects use `GameplayEffect`.

`On Fire`:
- applies a burning tag;
- deals periodic fire damage;
- has duration;
- can be removed by water.

### 8.4. Effect Removal

Effect removal should support:
- removal by another `GameplayEffect`;
- removal by gameplay tags.

Example:
- water applies an extinguish effect that removes burning effects by tag.

---

## 9. UI

Use Unreal built-in UI, without `CommonUI` for the initial implementation.

Recommended baseline:
- `UUserWidget`
- `UWidgetComponent` for overhead health display

Scaling note:
- `CommonUI` is a valid upgrade path if the UI layer grows.

---

## 10. Logging

Minimum important logs:
- weapon equipped;
- ammo slot changed;
- ammo definition equipped;
- fire requested and executed;
- reload started and completed;
- projectile hit;
- damage applied;
- healing applied;
- `On Fire` applied;
- `On Fire` removed;
- health depleted / death reached.

Rule:
- logs should make the gameplay sequence readable without deep debugging.

---

## 11. C++ / Blueprint Boundary

Keep in `C++`:
- combat logic;
- weapon logic;
- damage logic;
- status effect rules;
- gameplay state orchestration.

Use `Blueprint` for:
- data asset setup;
- visuals and content references;
- UI assembly;
- light integration.

---

## 12. Project Structure

All test-task code belongs in:

`Source/TestTask/TestTaskSolution/`

Recommended feature-based structure:

```text
Source/TestTask/TestTaskSolution/
  Core/
  Characters/
  PlayerState/
  Weapons/
    Components/
    Data/
    Abilities/
    Projectiles/
    Interfaces/
  Combat/
    Attributes/
    Effects/
    Executions/
  UI/
  Input/
  Logging/
```

Naming:
- project prefix: `Tt`
- examples:
  - `TtCharacter`
  - `TtPlayerState`
  - `TtWeaponComponent`
  - `TtAttributeSet`
  - `TtProjectile`

---

## 13. Minimum Expected Types

Core runtime types:
- `TtCharacter`
- `TtPlayerState`
- `TtWeaponComponent`
- `UTtWeaponInterface`
- `UTtAttributeSet`
- `ATtProjectile`

Core data/assets:
- `UTtWeaponDataAsset`
- `UTtProjectileDataAsset`

Core abilities/effects:
- `GA_Weapon_Fire`
- `GA_Weapon_Reload`
- `GA_Weapon_EquipAmmo`
- `GE_InitializeAttributes`
- `GE_Damage`
- `GE_Heal`
- `GE_Burning`
- `GE_Extinguish`

---

## 14. Scaling Notes

This design should scale toward:
- plugin-based feature extraction;
- `ExecutionCalculation` for more complex damage;
- richer weapon and inventory persistence on top of `PlayerState + ASC`;
- `CommonUI` if UI complexity increases;

Temporary simplifications allowed in the first version:
- exactly four ammo slots via `ETtAmmoSlot`;
- simple overhead health UI;
- relatively simple damage formulas;
- configurable reload input if not fixed by the task.

