// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/FPSWidgetController.h"
#include "Weapon/FPSWeaponBase.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangedSignature, float, NewValue);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FPS_API UOverlayWidgetController : public UFPSWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbasksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeChangedSignature OnDamageTaken;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable,Category="Weapon|Ammo")
	FOnAmmoChangedSignature OnCurrentAmmoChanged;
	UPROPERTY(BlueprintAssignable,Category="Weapon|Ammo")
	FOnAmmoChangedSignature OnReserveAmmoChanged;
	UPROPERTY(BlueprintAssignable,Category="Weapon|FireMode")
	FOnFireModeChangedSignature OnFireModeChanged;

	UFUNCTION()
	void HandleCurrentAmmoChanged(int32 NewAmmo);
	UFUNCTION()
	void HandleReserveAmmoChanged(int32 NewAmmo);
	UFUNCTION()
	void HandleFireModeChanged(EWeaponFireMode NewFireMode);
protected:
	
};
