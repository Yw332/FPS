// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class FPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AFPSPlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	//context
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	//action
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> StartFireAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> GrenadeAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InteractionAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AimAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SwitchFireModeAction;
	float SprintAlphaValue = 0.f;
	
	
	//function
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void StartFire1(const FInputActionValue& InputActionValue);
	void StopFire(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	void Lean(const FInputActionValue& InputActionValue);
	void StartAim(const FInputActionValue& InputActionValue);
	void StopAim(const FInputActionValue& InputActionValue);
	void Reload(const FInputActionValue& InputActionValue);
	void Sprint(const FInputActionValue& InputActionValue);
	void SwitchFireMode(const FInputActionValue& InputActionValue);
};
