# TestTaskSolution

`TestTaskSolution` is a test `WeaponSystem` project with weapon and ammo switching, projectile firing, reloading, damage processing, resistances, and status effects.
The implementation is based on `Gameplay Ability System` with event-driven ability activation and data-driven weapon/ammo setup.

# Showcases

- Node Wrap : https://youtu.be/ojC33S3Y9S4

## Implemented Features

- Projectile shooting through GAS (`Event -> Ability -> Projectile -> GameplayEffect`).
- 4 ammo types: regular, fire, water, healing.
- Burning status (periodic damage) and water-based extinguish flow via effects/tags.
- Resistances (`Physical`, `Fire`, `Water`) and final damage calculation in custom `Magnitude Calculation` classes.
- Attribute initialization through `GameplayEffect`.
- Blueprint HUD/widgets (`WBP_Hud`, `WBP_HealthBar`) fed by C++ gameplay state.

## Architecture

### Core

- `ATtGameMode` defines the runtime set: `Player`, `PlayerController`, `PlayerState`.
- `TtGameplayTags` is the single tag dictionary for events, ammo identities, and damage data tags.

### Characters + State

- `ATtCharacter`: base character with shared GAS integration.
- `ATtPlayer`: player pawn, input entry point, implements `ITtWeaponInterface`.
- `ATtEnemy`: NPC target with local ASC, health bar, and reaction to `Status.Burning`.
- `ATtPlayerState`: persistent owner of the player ASC.

### GAS

- `UTtAbilitySystemComponent`: actor info + `AttributeSet` initialization.
- `UTtAttributeSet`: `Health/MaxHealth`, resistances, and `Damage` as a base data attribute.
- Abilities:
  - `UTtFireProjectileAbility`
  - `UTtReloadAbility`
  - `UTtEquipWeaponAbility`
  - `UTtEquipAmmoAbility`
- Combat calculations:
  - `UTtPhysicalDamageCalculation`
  - `UTtFireDamageCalculation`
  - `UTtWaterDamageCalculation`
  - `UTtHealingCalculation`

### Weapons (Data-Driven)

- `UTtWeaponComponent` stores and orchestrates:
  - current weapon slot;
  - current ammo slot;
  - `WeaponSlot -> WeaponData`;
  - `AmmoSlot -> AmmoData`;
  - `WeaponSlot -> AmmoInMag`;
  - `WeaponSlot -> LastSelectedAmmoSlot`.
- `UTtWeaponData`, `UTtAmmoData`, `UTtProjectileData`: DataAsset-based configuration.
- `ATtProjectile`: movement, hit handling, and hit-effect application.

## Key Decisions and Why

- **Player ASC is on `PlayerState`, not on `Character`**. This keeps gameplay state (effects/cooldowns/attributes) stable across pawn replacement and simplifies network consistency.
- **Event-driven activation via gameplay tags (`SendGameplayEvent`)**. Input, UI, and gameplay logic stay loosely coupled, so abilities can be replaced without rewriting input flow.
- **`WeaponHandler/WeaponMesh` is separated from `WeaponComponent`**. Visuals and combat logic are isolated, reducing regression risk when changing weapon presentation.
- **DataAsset-first weapon/ammo setup**. New gameplay combinations are added through data rather than branching gameplay code.
- **Damage logic is split into dedicated calculation classes**. Type-specific formulas are isolated and easier to iterate for balancing.

## How to Run and Verify

1. Open `TestTask.uproject` in Unreal Engine 5.
2. Ensure the main setup uses:
   - `GM_TtGameMode` (`Content/TestTask/Core/GM_TtGameMode.uasset`);
   - `BP_TtPlayer` (`Content/TestTask/Characters/BP_TtPlayer.uasset`);
   - `PS_TtPlayerState` (`Content/TestTask/PlayerState/PS_TtPlayerState.uasset`);
   - `PC_TtPlayerController` (`Content/TestTask/PlayerController/PC_TtPlayerController.uasset`).
3. Run PIE and verify:
   - `LMB`: fire;
   - `R`: reload;
   - `1..4`: switch ammo type;
   - weapon switching (if `IA_EquipNextWeapon/IA_EquipPrevWeapon` is assigned).
4. Open `Output Log` and filter by `LogTestTask`.

## Working with the Project

### Add a New Weapon

1. Create a `UTtWeaponData` asset in `Content/TestTask/Data/Weapons/`.
2. Configure `MagazineSize`, `BasicDamage`, abilities/effects, and `WeaponMesh`.
3. Add it to `DefaultWeaponSlotsByData` in `BP_TtPlayer`.

### Add a New Ammo Type

1. Create `UTtProjectileData` and `UTtAmmoData` assets in `Content/TestTask/Data/Projectiles` and `Content/TestTask/Data/Ammo`.
2. Configure `OnHitEffectClass` and `AmmoTag`.
3. Add the ammo asset to `DefaultAmmoSlotsByData`.

### Change Damage Formula

1. Open the needed calculation class in `Combat/Execution`.
2. Adjust resistance weights/base formula.
3. Validate behavior in PIE and `Output Log`.

## Logging

- Category: `LogTestTask` (`Source/TestTask/TestTask.h`).
- Logs cover the full runtime flow:
  - ASC/AttributeSet initialization;
  - ability granting;
  - equip/fire/reload requests and rejection reasons;
  - projectile spawn/hit;
  - hit-effect application;
  - ammo/slot state changes;
  - health changes, damage, healing, death;
  - enemy burning status transitions.

## Scaling Opportunities

- **CommonUI**: current UI is based on `UUserWidget`, which is fine for test scope; for larger UI flows (inventory, loadout, platform-specific input hints), migrate to `CommonUI` for standardized navigation, action routing, and reusable controls.
- **CurveTable damage integration**: damage coefficients are currently hardcoded in calculation classes; the next step is to externalize scaling to `CurveTable` (for weapon level, target type, status stacks, etc.) so balance changes can be done in data without recompilation.
- The slot + DataAsset model is already extension-friendly: new weapon/ammo variants can be added without changing ability contracts.
- The combat stack can be extracted into a plugin module later with the same external API (`WeaponComponent + GameplayTags + DataAssets`).
