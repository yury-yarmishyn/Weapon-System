// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

void ATtPlayer::Look(const FVector2D& LookInput)
{
	if (!Controller)
	{
		return;
	}

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(-LookInput.Y);
}
