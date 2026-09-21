// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/FPSGameModeBase.h"

void AFPSGameModeBase::PlayerDied()
{
	OnPlayerDied();
}

void AFPSGameModeBase::EnemyDied(APawn* Enemy)
{
	OnEnemyDied(Enemy);
}


