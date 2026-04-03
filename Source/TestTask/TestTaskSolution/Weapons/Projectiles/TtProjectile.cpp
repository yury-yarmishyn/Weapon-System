// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestTaskSolution/Weapons/Projectiles/TtProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "TestTask.h"
#include "TestTaskSolution/Weapons/Data/TtProjectileData.h"

ATtProjectile::ATtProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(8.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	SetRootComponent(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 2500.f;
	ProjectileMovementComponent->MaxSpeed = 2500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
}

void ATtProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* ProjectileOwner = GetOwner())
	{
		CollisionComponent->IgnoreActorWhenMoving(ProjectileOwner, true);
	}

	if (ProjectileData)
	{
		CollisionComponent->SetSphereRadius(ProjectileData->CollisionRadius);
		ProjectileMovementComponent->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = ProjectileData->MaxSpeed;
		ProjectileMovementComponent->ProjectileGravityScale = ProjectileData->GravityScale;
		ProjectileMovementComponent->bShouldBounce = ProjectileData->bShouldBounce;
		LifeSeconds = ProjectileData->LifeSeconds;
		bDestroyOnImpact = ProjectileData->bDestroyOnHit;
	}

	CollisionComponent->OnComponentHit.AddDynamic(this, &ATtProjectile::HandleCollisionHit);
	SetLifeSpan(LifeSeconds);
}

void ATtProjectile::HandleCollisionHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	(void)HitComponent;
	(void)OtherComp;
	(void)NormalImpulse;

	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}

	UE_LOG(
		LogTestTask,
		Log,
		TEXT("Projectile hit: Projectile=%s Target=%s"),
		*GetNameSafe(this),
		*GetNameSafe(OtherActor));

	FTtProjectileImpactContext ImpactContext;
	BuildImpactContext(OtherActor, Hit, ImpactContext);
	ForwardImpactToEffectPipeline(ImpactContext);

	if (bDestroyOnImpact)
	{
		Destroy();
	}
}

void ATtProjectile::BuildImpactContext(AActor* HitActor, const FHitResult& HitResult, FTtProjectileImpactContext& OutContext) const
{
	OutContext.ProjectileOwner = GetOwner();
	OutContext.ProjectileInstigator = GetInstigator();
	OutContext.HitActor = HitActor;
	OutContext.HitResult = HitResult;
}

void ATtProjectile::ForwardImpactToEffectPipeline(const FTtProjectileImpactContext& ImpactContext)
{
	UE_LOG(
		LogTestTask,
		Log,
		TEXT("Projectile impact forwarded to effect pipeline. Projectile=%s Target=%s Owner=%s Instigator=%s ImpactPoint=%s Bone=%s"),
		*GetNameSafe(this),
		*GetNameSafe(ImpactContext.HitActor),
		*GetNameSafe(ImpactContext.ProjectileOwner),
		*GetNameSafe(ImpactContext.ProjectileInstigator),
		*ImpactContext.HitResult.ImpactPoint.ToCompactString(),
		*ImpactContext.HitResult.BoneName.ToString());
}
