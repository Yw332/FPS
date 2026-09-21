// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void PlayerDied();
	UFUNCTION(BlueprintCallable)
	void EnemyDied(APawn* Enemy);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerDied();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyDied(APawn* Enemy);
};
