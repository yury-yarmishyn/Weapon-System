// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Characters/TtEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "TestTask.h"
#include "TestTaskSolution/Combat/AbilityComponent/TtAbilitySystemComponent.h"
#include "TestTaskSolution/Core/TtGameplayTags.h"

ATtEnemy::ATtEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UTtAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetMesh());
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
	HealthBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATtEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AbilitySystemComponent && BurningTagChangedHandle.IsValid())
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(
			TtGameplayTags::TAG_Status_Burning,
			EGameplayTagEventType::NewOrRemoved).Remove(BurningTagChangedHandle);

		BurningTagChangedHandle.Reset();
	}

	HandleBurningTagChanged(TtGameplayTags::TAG_Status_Burning, 0);

	Super::EndPlay(EndPlayReason);
}

void ATtEnemy::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTestTask, Error, TEXT("[%s] InitializeAbilitySystem failed: AbilitySystemComponent is null"), *GetNameSafe(this));
		return;
	}

	AbilitySystemComponent->InitializeAbilitySystemComponent(this, this);
	UE_LOG(LogTestTask, Log, TEXT("[%s] Enemy ASC initialized"), *GetNameSafe(this));
	BindBurningTagEvents();

	if (!HasAuthority())
	{
		return;
	}

	AbilitySystemComponent->InitializeAttributeSet();
	UE_LOG(LogTestTask, Log, TEXT("[%s] Enemy AttributeSet initialization requested"), *GetNameSafe(this));
}

void ATtEnemy::BindBurningTagEvents()
{
	if (!AbilitySystemComponent || BurningTagChangedHandle.IsValid())
	{
		return;
	}

	BurningTagChangedHandle = AbilitySystemComponent->RegisterGameplayTagEvent(
		TtGameplayTags::TAG_Status_Burning,
		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ATtEnemy::HandleBurningTagChanged);

	HandleBurningTagChanged(
		TtGameplayTags::TAG_Status_Burning,
		AbilitySystemComponent->GetTagCount(TtGameplayTags::TAG_Status_Burning));
}

void ATtEnemy::HandleBurningTagChanged(FGameplayTag InTag, int32 NewCount)
{
	(void)InTag;
	USkeletalMeshComponent* MeshComponent = GetMesh();
	if (!MeshComponent)
	{
		return;
	}

	static const FName BurningTintParameterName(TEXT("Paint Tint"));
	static const FVector BurningTintColor(1.0f, 0.15f, 0.1f);
	static const FVector DefaultTintColor(1.0f, 1.0f, 1.0f);
	const bool bEnable = NewCount > 0;

	MeshComponent->SetVectorParameterValueOnMaterials(
		BurningTintParameterName,
		bEnable ? BurningTintColor : DefaultTintColor);

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Burning status changed: %s (TagCount=%d)"),
		*GetNameSafe(this),
		bEnable ? TEXT("active") : TEXT("inactive"),
		NewCount);
}
