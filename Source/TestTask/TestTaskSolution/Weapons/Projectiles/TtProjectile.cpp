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
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetGenerateOverlapEvents(false);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetHiddenInGame(false);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ATtProjectile::OnCollisionHit);

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

	AActor* InitiatorActor = GetInstigator();
	if (!InitiatorActor)
	{
		InitiatorActor = GetOwner();
	}

	if (InitiatorActor)
	{
		CollisionComponent->IgnoreActorWhenMoving(InitiatorActor, true);
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
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	(void)HitComponent;
	(void)OtherComp;
	(void)NormalImpulse;
	(void)Hit;

	AActor* InitiatorActor = GetInstigator();
	if (!InitiatorActor)
	{
		InitiatorActor = GetOwner();
	}

	if (!OtherActor || OtherActor == this || OtherActor == InitiatorActor)
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

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
	if (!TargetASC)
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("[%s] On-hit effect skipped: target %s has no ASC"),
			*GetNameSafe(this),
			*GetNameSafe(HitActor));
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
		UE_LOG(
			LogTestTask,
			Verbose,
			TEXT("[%s] Applied effect via fallback TargetASC->ApplyGameplayEffectToSelf"),
			*GetNameSafe(this));
		return;
	}

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(ProjectileData->OnHitEffectClass, 1.f, Context);
	if (!SpecHandle.IsValid())
	{
		UE_LOG(
			LogTestTask,
			Warning,
			TEXT("[%s] Failed to create outgoing spec for effect %s"),
			*GetNameSafe(this),
			*EffectClassName);
		return;
	}

	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	UE_LOG(
		LogTestTask,
		Verbose,
		TEXT("[%s] Effect spec applied to target ASC"),
		*GetNameSafe(this));
}
