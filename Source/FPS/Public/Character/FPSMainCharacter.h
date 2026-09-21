// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FPSCharacterBase.h"
#include "FPSMainCharacter.generated.h"
class AFPSWeaponBase;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class FPS_API AFPSMainCharacter : public AFPSCharacterBase
{
	GENERATED_BODY()
public:
	AFPSMainCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual void BindCallbacksToDependencies() override;
	virtual void InitAbilityActorInfo() override;
	virtual void OnDeath() override;
	
	UPROPERTY(EditDefaultsOnly,Category="Character|Combat")
	TSubclassOf<AFPSWeaponBase> WeaponClass;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character|Combat")
	TObjectPtr<AFPSWeaponBase> Weapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character|Mesh")
	TObjectPtr<USkeletalMeshComponent> FirstPersonArm;


	
	UPROPERTY(BlueprintReadOnly, Category="Character|Aim")
	bool bIsAiming = false;
	UPROPERTY(EditAnywhere, Category="Character|Aim")
	float DefaultFOV = 90.f;
	UPROPERTY(EditAnywhere, Category="Character|Aim")
	float AimingFOV = 30.f;
	UPROPERTY(EditAnywhere, Category="Character|Aim")
	float AimFOVInterpSpeed = 12.f;
	
	UFUNCTION()
	void StartFire();
	UFUNCTION()
	void StopFire();
	UFUNCTION()
	void Reload();
	UFUNCTION()
	void StartAim();
	UFUNCTION()
	void StopAim();
	UFUNCTION()
	void SwitchFireMode();
	void SpawnWeapon();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
protected:

private:
	
	
};


