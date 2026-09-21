// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/FPSWidgetController.h"
#include "Enemy/FPSEnemyBase.h"
#include "EnemyWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UEnemyWidgetController : public UFPSWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbasksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnEnemyAttributeChanged OnMaxHealthChanged;
protected:
	
};
