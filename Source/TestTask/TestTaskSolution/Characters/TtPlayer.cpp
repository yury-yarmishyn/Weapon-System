// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtPlayer.h"
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
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	CameraComponent->bUsePawnControlRotation = true;

	WeaponHandler = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponHandler"));
	WeaponHandler->SetupAttachment(CameraComponent);
	WeaponHandler->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponComponent = CreateDefaultSubobject<UTtWeaponComponent>(TEXT("WeaponComponent"));
}

void ATtPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystemFromPlayerState();
}

void ATtPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystemFromPlayerState();
}

void ATtPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystemFromPlayerState();
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

USkeletalMeshComponent* ATtPlayer::GetWeaponHandler() const
{
	return WeaponHandler;
}

UTtWeaponComponent* ATtPlayer::GetWeaponComponent() const
{
	return WeaponComponent;
}

void ATtPlayer::InitializeAbilitySystemFromPlayerState()
{
	ATtPlayerState* TtPlayerState = GetPlayerState<ATtPlayerState>();
	if (!TtPlayerState)
	{
		return;
	}

	UTtAbilitySystemComponent* TtAbilitySystemComponent = TtPlayerState->GetTtAbilitySystemComponent();
	if (!TtAbilitySystemComponent)
	{
		return;
	}

	TtAbilitySystemComponent->InitializeAbilitySystemComponent(TtPlayerState, this);
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

	WeaponComponent->Fire(nullptr);
}

void ATtPlayer::ReloadInput()
{
	if (!WeaponComponent)
	{
		return;
	}

	WeaponComponent->Reload(WeaponComponent->GetCurrentWeaponSlot());
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

void ATtPlayer::EquipAmmoInput(const ETtAmmoSlot AmmoSlot)
{
	if (!WeaponComponent)
	{
		return;
	}

	WeaponComponent->EquipAmmoBySlot(AmmoSlot);
}
