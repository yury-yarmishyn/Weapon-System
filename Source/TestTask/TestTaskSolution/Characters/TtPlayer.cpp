// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtPlayer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputActionValue.h"
#include "TestTask.h"
#include "TestTaskSolution/Combat/AbilityComponent/TtAbilitySystemComponent.h"
#include "TestTaskSolution/PlayerState/TtPlayerState.h"
#include "TestTaskSolution/Weapons/Components/TtWeaponComponent.h"

ATtPlayer::ATtPlayer()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetMesh());
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	CameraComponent->bUsePawnControlRotation = true;

	WeaponHandler = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponHandler"));
	WeaponHandler->SetupAttachment(CameraComponent);
	WeaponHandler->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(WeaponHandler);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponComponent = CreateDefaultSubobject<UTtWeaponComponent>(TEXT("WeaponComponent"));
}

void ATtPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitializeWeaponComponent();
}

void ATtPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();
	InitializeWeaponComponent();
}

void ATtPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();
	InitializeWeaponComponent();
}

void ATtPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATtPlayer::MoveInput);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATtPlayer::LookInput);
		}

		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATtPlayer::FireInput);
		}

		if (ReloadAction)
		{
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ATtPlayer::ReloadInput);
		}

		if (AmmoSlot1Action)
		{
			EnhancedInputComponent->BindAction(AmmoSlot1Action, ETriggerEvent::Started, this, &ATtPlayer::AmmoSlot1Input);
		}

		if (AmmoSlot2Action)
		{
			EnhancedInputComponent->BindAction(AmmoSlot2Action, ETriggerEvent::Started, this, &ATtPlayer::AmmoSlot2Input);
		}

		if (AmmoSlot3Action)
		{
			EnhancedInputComponent->BindAction(AmmoSlot3Action, ETriggerEvent::Started, this, &ATtPlayer::AmmoSlot3Input);
		}

		if (AmmoSlot4Action)
		{
			EnhancedInputComponent->BindAction(AmmoSlot4Action, ETriggerEvent::Started, this, &ATtPlayer::AmmoSlot4Input);
		}

		if (NextWeaponAction)
		{
			EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Started, this, &ATtPlayer::NextWeaponInput);
		}

		if (PrevWeaponAction)
		{
			EnhancedInputComponent->BindAction(PrevWeaponAction, ETriggerEvent::Started, this, &ATtPlayer::PrevWeaponInput);
		}
		
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATtPlayer::Jump);
		}
	}
	else
	{
		UE_LOG(LogTestTask, Error, TEXT("'%s' Failed to find an Enhanced Input Component."), *GetNameSafe(this));
	}
}

USkeletalMeshComponent* ATtPlayer::GetWeaponHandler_Implementation() const
{
	return WeaponHandler;
}

USkeletalMeshComponent* ATtPlayer::GetWeaponMesh_Implementation() const
{
	return WeaponMesh;
}

UTtWeaponComponent* ATtPlayer::GetWeaponComponent_Implementation() const
{
	return WeaponComponent;
}

void ATtPlayer::InitializeAbilitySystem()
{
	ATtPlayerState* TtPlayerState = GetPlayerState<ATtPlayerState>();
	if (!TtPlayerState)
	{
		UE_LOG(LogTestTask, Verbose, TEXT("[%s] InitializeAbilitySystem skipped: PlayerState is null"), *GetNameSafe(this));
		return;
	}

	UTtAbilitySystemComponent* TtAbilitySystemComponent = TtPlayerState->GetTtAbilitySystemComponent();
	if (!TtAbilitySystemComponent)
	{
		UE_LOG(LogTestTask, Error, TEXT("[%s] InitializeAbilitySystem failed: TtAbilitySystemComponent is null"), *GetNameSafe(this));
		return;
	}

	TtAbilitySystemComponent->InitializeAbilitySystemComponent(TtPlayerState, this);
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] InitializeAbilitySystem completed. Owner=%s Avatar=%s"),
		*GetNameSafe(this),
		*GetNameSafe(TtPlayerState),
		*GetNameSafe(this));
}

void ATtPlayer::InitializeWeaponComponent()
{
	if (!WeaponComponent)
	{
		return;
	}

	if (!bWeaponComponentInitialized)
	{
		WeaponComponent->InitWeaponComponent(DefaultWeaponSlotsByData, DefaultAmmoSlotsByData);
		bWeaponComponentInitialized = true;

		UE_LOG(
			LogTestTask,
			Log,
			TEXT("[%s] WeaponComponent initialized. Weapon slots: %d, ammo slots: %d"),
			*GetNameSafe(this),
			DefaultWeaponSlotsByData.Num(),
			DefaultAmmoSlotsByData.Num());
	}

	if (!HasAuthority() || bWeaponAbilitiesGranted)
	{
		return;
	}

	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this))
	{
		return;
	}

	WeaponComponent->GrantAbilitiesFromInitData();
	bWeaponAbilitiesGranted = true;
}

void ATtPlayer::Move(const FVector2D& MoveInput)
{
	if (!Controller)
	{
		return;
	}

	AddMovementInput(GetActorRightVector(), MoveInput.X);
	AddMovementInput(GetActorForwardVector(), MoveInput.Y);
}

void ATtPlayer::MoveInput(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	Move(MoveVector);
}

void ATtPlayer::Look(const FVector2D& LookInput)
{
	if (!Controller)
	{
		return;
	}

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(-LookInput.Y);
}

void ATtPlayer::LookInput(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	Look(LookVector);
}

void ATtPlayer::FireInput()
{
	if (!WeaponComponent)
	{
		return;
	}

	const ETtWeaponSlot CurrentWeaponSlot = WeaponComponent->GetCurrentWeaponSlot();
	ETtAmmoSlot CurrentAmmoSlot = WeaponComponent->GetCurrentAmmoSlot();
	if (const ETtAmmoSlot* WeaponAmmoSlot = WeaponComponent->GetWeaponSlotByAmmoSlot().Find(CurrentWeaponSlot))
	{
		CurrentAmmoSlot = *WeaponAmmoSlot;
	}

	WeaponComponent->FireBySlot(CurrentAmmoSlot);
}

void ATtPlayer::ReloadInput()
{
	if (!WeaponComponent)
	{
		return;
	}

	const ETtWeaponSlot CurrentWeaponSlot = WeaponComponent->GetCurrentWeaponSlot();
	if (!WeaponComponent->CanEquipWeapon(CurrentWeaponSlot))
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("[%s] Reload input ignored: weapon slot %d is not equipped"),
			*GetNameSafe(this),
			static_cast<int32>(CurrentWeaponSlot));
		return;
	}

	WeaponComponent->Reload(CurrentWeaponSlot);
}

void ATtPlayer::AmmoSlot1Input()
{
	EquipAmmoInput(ETtAmmoSlot::Slot1);
}

void ATtPlayer::AmmoSlot2Input()
{
	EquipAmmoInput(ETtAmmoSlot::Slot2);
}

void ATtPlayer::AmmoSlot3Input()
{
	EquipAmmoInput(ETtAmmoSlot::Slot3);
}

void ATtPlayer::AmmoSlot4Input()
{
	EquipAmmoInput(ETtAmmoSlot::Slot4);
}

void ATtPlayer::NextWeaponInput()
{
	if (!WeaponComponent)
	{
		return;
	}

	const ETtWeaponSlot NextWeaponSlot = WeaponComponent->GetNextWeapon(WeaponComponent->GetCurrentWeaponSlot());
	WeaponComponent->EquipWeaponBySlot(NextWeaponSlot);
}

void ATtPlayer::PrevWeaponInput()
{
	if (!WeaponComponent)
	{
		return;
	}

	const ETtWeaponSlot PrevWeaponSlot = WeaponComponent->GetPrevWeapon(WeaponComponent->GetCurrentWeaponSlot());
	WeaponComponent->EquipWeaponBySlot(PrevWeaponSlot);
}

void ATtPlayer::EquipAmmoInput(const ETtAmmoSlot AmmoSlot)
{
	if (!WeaponComponent)
	{
		return;
	}

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Ammo slot input received: %d"),
		*GetNameSafe(this),
		static_cast<int32>(AmmoSlot));
	WeaponComponent->EquipAmmoBySlot(AmmoSlot);
}
