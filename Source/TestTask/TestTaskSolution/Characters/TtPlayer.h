// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TtCharacter.h"
#include "TestTaskSolution/Weapons/Interfaces/TtWeaponInterface.h"
#include "TtPlayer.generated.h"

enum class ETtAmmoSlot : uint8;
class UCameraComponent;
class USkeletalMeshComponent;
class UTtWeaponComponent;
class UInputAction;
class UInputComponent;
struct FInputActionValue;

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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void FireInput();
	void ReloadInput();
	void AmmoSlot1Input();
	void AmmoSlot2Input();
	void AmmoSlot3Input();
	void AmmoSlot4Input();
	void EquipAmmoInput(ETtAmmoSlot AmmoSlot);

protected:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> FireAction = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ReloadAction = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AmmoSlot1Action = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AmmoSlot2Action = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AmmoSlot3Action = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AmmoSlot4Action = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> NextWeaponAction = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> PrevWeaponAction = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction = nullptr;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction = nullptr;

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
