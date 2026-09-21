// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FPSAttributeSet.h"
#include "Gameplay/FPSGameModeBase.h"
// Sets default values
AFPSCharacterBase::AFPSCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bIsDead = false;
}

UAbilitySystemComponent* AFPSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSCharacterBase::BindCallbacksToDependencies()
{
	
}

void AFPSCharacterBase::OnDeath()
{
	
}

void AFPSCharacterBase::InitAbilityActorInfo()
{
	
}

void AFPSCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	if (!GetAbilitySystemComponent())
	{
		return;
	}
	if (!GameplayEffectClass)
	{
		return;
	}
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}


void AFPSCharacterBase::InitDefaultAttribute() const
{
	ApplyEffectToSelf(DefaultPrimaryAttribute,1);
	ApplyEffectToSelf(DefaultSecondaryAttribute,1);
}







