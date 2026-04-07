// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Weapons/Projectiles/TtProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayEffect.h"
#include "TestTask.h"
#include "TestTaskSolution/Weapons/Data/TtProjectileData.h"

ATtProjectile::ATtProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	CollisionComponent->SetSphereRadius(8.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetHiddenInGame(false);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATtProjectile::OnCollisionHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 2500.f;
	ProjectileMovement->MaxSpeed = 2500.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
}

void ATtProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* InstigatorActor = GetInstigator())
	{
		CollisionComponent->IgnoreActorWhenMoving(InstigatorActor, true);
	}

	InitProjectile();
}

void ATtProjectile::InitProjectile()
{
	if (!ProjectileData)
	{
		UE_LOG(LogTestTask, Warning, TEXT("Projectile %s has no ProjectileData assigned"), *GetName());
		return;
	}

	if (CollisionComponent)
	{
		CollisionComponent->SetSphereRadius(ProjectileData->CollisionRadius);
		CollisionComponent->ShapeColor = ProjectileData->ProjectileColor.ToFColor(true);
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovement->MaxSpeed = ProjectileData->MaxSpeed;
		ProjectileMovement->ProjectileGravityScale = ProjectileData->GravityScale;
		ProjectileMovement->bShouldBounce = ProjectileData->bShouldBounce;

		const FVector LaunchVelocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;
		ProjectileMovement->Velocity = LaunchVelocity;
	}

	if (ProjectileData->LifeSeconds > 0.f)
	{
		SetLifeSpan(ProjectileData->LifeSeconds);
	}
}

void ATtProjectile::OnCollisionHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	(void)OverlappedComponent;
	(void)OtherComp;
	(void)OtherBodyIndex;
	(void)bFromSweep;
	(void)SweepResult;

	if (!OtherActor || OtherActor == this || OtherActor == GetInstigator() || OtherActor == GetOwner())
	{
		return;
	}

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Projectile hit actor %s"),
		*GetNameSafe(this),
		*GetNameSafe(OtherActor));

	ApplyOnHitEffect(OtherActor);
	Destroy();
}

void ATtProjectile::ApplyOnHitEffect(AActor* HitActor) const
{
	if (!HitActor || !ProjectileData || !ProjectileData->OnHitEffectClass)
	{
		return;
	}

	const FString EffectClassName = GetNameSafe(ProjectileData->OnHitEffectClass);
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("[%s] Applying on-hit effect %s to %s"),
		*GetNameSafe(this),
		*EffectClassName,
		*GetNameSafe(HitActor));

	if (EffectClassName.Contains(TEXT("FireHit")))
	{
		UE_LOG(LogTestTask, Log, TEXT("[%s] On Fire requested"), *GetNameSafe(HitActor));
	}
	else if (EffectClassName.Contains(TEXT("WaterHit")))
	{
		UE_LOG(LogTestTask, Log, TEXT("[%s] On Fire removal requested"), *GetNameSafe(HitActor));
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
	if (!TargetASC)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
	if (!SourceASC)
	{
		SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	}

	if (!SourceASC)
	{
		FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
		Context.AddSourceObject(this);
		TargetASC->ApplyGameplayEffectToSelf(ProjectileData->OnHitEffectClass->GetDefaultObject<UGameplayEffect>(), 1.f, Context);
		return;
	}

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(ProjectileData->OnHitEffectClass, 1.f, Context);
	if (!SpecHandle.IsValid())
	{
		return;
	}

	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}
