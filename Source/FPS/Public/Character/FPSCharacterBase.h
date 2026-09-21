// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "FPSCharacterBase.generated.h"

class UFPSAttributeSet;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeFloatChanged, float, NewValue);//基类统一实现广播
UCLASS(Abstract)
class FPS_API AFPSCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const{return AttributeSet;}
	virtual void BeginPlay() override;
	virtual void BindCallbacksToDependencies();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsDead;
	UFUNCTION(BlueprintCallable)
	virtual void OnDeath();

	UPROPERTY(BlueprintAssignable)
	FOnAttributeFloatChanged OnHealthInitialized;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeFloatChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeFloatChanged OnMaxHealthInitialized;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeFloatChanged OnMaxHealthChanged;
    
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UFUNCTION()
	virtual void InitAbilityActorInfo();

	//GE
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttribute;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttribute;

	UFUNCTION()
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level) const;
	UFUNCTION()
	void InitDefaultAttribute() const;
	
	
private:
	
};
