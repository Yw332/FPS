// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/FPSAttributeSet.h"
#include "Character/FPSMainCharacter.h"
#include "Weapon/FPSWeaponBase.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	UFPSAttributeSet* FPSAttributeSet = Cast<UFPSAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(FPSAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(FPSAttributeSet->GetMaxHealth());

	if (AFPSMainCharacter* FPSCharacter = Cast<AFPSMainCharacter>(PlayerController->GetPawn()))
	{
		if (AFPSWeaponBase* FPSWeapon = FPSCharacter->Weapon)
		{
			OnCurrentAmmoChanged.Broadcast(FPSWeapon->CurrentAmmo);
			OnReserveAmmoChanged.Broadcast(FPSWeapon->ReServeAmmo);
			OnFireModeChanged.Broadcast(FPSWeapon->FireMode);
		}
	}	
}

void UOverlayWidgetController::BindCallbasksToDependencies()
{
	UFPSAttributeSet* FPSAttributeSet = Cast<UFPSAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FPSAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)	
		{
			OnHealthChanged.Broadcast(Data.NewValue);
			if (Data.NewValue<Data.OldValue)//受伤多播实现属性初始化与后续改变的区别，另外一种在FPSEnemyBase.cpp中实现
			{
				OnDamageTaken.Broadcast(Data.OldValue-Data.NewValue);
			}
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FPSAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	if (AFPSMainCharacter* FPSCharacter = Cast<AFPSMainCharacter>(PlayerController->GetPawn()))
	{
		if (AFPSWeaponBase* FPSWeapon = FPSCharacter->Weapon)
		{
			FPSWeapon->OnCurrentAmmoChanged.AddDynamic(this,&UOverlayWidgetController::HandleCurrentAmmoChanged);
			FPSWeapon->OnReserveAmmoChanged.AddDynamic(this,&UOverlayWidgetController::HandleReserveAmmoChanged);
			FPSWeapon->OnFireModeChanged.AddDynamic(this,&UOverlayWidgetController::HandleFireModeChanged);
		}
	}
}

void UOverlayWidgetController::HandleCurrentAmmoChanged(int32 NewAmmo)
{
	OnCurrentAmmoChanged.Broadcast(NewAmmo);
}

void UOverlayWidgetController::HandleReserveAmmoChanged(int32 NewAmmo)
{
	OnReserveAmmoChanged.Broadcast(NewAmmo);
}

void UOverlayWidgetController::HandleFireModeChanged(EWeaponFireMode NewFireMode)
{
	OnFireModeChanged.Broadcast(NewFireMode);
}
