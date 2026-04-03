// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TtCharacter.h"
#include "TestTaskSolution/Weapons/Interfaces/TtWeaponInterface.h"
#include "TtPlayer.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class UTtWeaponComponent;

UCLASS()
class TESTTASK_API ATtPlayer : public ATtCharacter, public ITtWeaponInterface
{
	GENERATED_BODY()

public:
	ATtPlayer();
	virtual USkeletalMeshComponent* GetWeaponHandler() const override;
	virtual UTtWeaponComponent* GetWeaponComponent() const override;

	UFUNCTION(BlueprintCallable, Category="Input")
	void Move(const FVector2D& MoveInput);

	UFUNCTION(BlueprintCallable, Category="Input")
	void Look(const FVector2D& LookInput);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitializeAbilitySystemFromPlayerState();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> WeaponHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UTtWeaponComponent> WeaponComponent;
};
