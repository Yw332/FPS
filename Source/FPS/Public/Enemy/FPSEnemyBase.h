// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FPSCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "FPSEnemyBase.generated.h"

class UWidgetComponent;
class UFPSUserWidget;
struct FWidgetControllerParams;
class UEnemyWidgetController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyAttributeChanged, float, NewValue);
/**
 * 
 */
UCLASS()
class FPS_API AFPSEnemyBase : public AFPSCharacterBase
{
	GENERATED_BODY()
public:
	AFPSEnemyBase();
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(EditAnywhere,Category="Enemy|Combat")
	UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere,Category="Enemy|Combat")
	float HitStunDuration = 0.3f;
	FTimerHandle StunTimerHandle;
	
	UPROPERTY(EditAnywhere,Category="Enemy|GameplayEffects")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;
	UFUNCTION(BlueprintCallable,Category="Enemy|Attack")
	void DoAttackDamage();
	virtual void OnDeath() override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeSet> DebugAttributeSet;
protected:
	
};
