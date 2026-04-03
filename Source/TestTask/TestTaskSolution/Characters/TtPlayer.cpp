// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputActionValue.h"
#include "TestTask.h"
#include "TestTaskSolution/Core/TtAbilitySystemComponent.h"
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

void ATtPlayer::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	AddMovementInput(GetActorRightVector(), MovementVector.X);
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
}

void ATtPlayer::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
