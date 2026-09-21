// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"
class UFPSUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class FPS_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
public:
	UOverlayWidgetController* GetWidgetController(const FWidgetControllerParams& Params);
	void InitOverlay(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widgets")
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere,Category="Widgets")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widgets")
	TObjectPtr<UFPSUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere,Category="Widgets")
	TSubclassOf<UFPSUserWidget> OverlayWidgetClass;

	
protected:

private:

};
