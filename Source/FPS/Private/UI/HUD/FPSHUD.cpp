// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FPSHUD.h"

#include "UI/Widget/FPSUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AFPSHUD::GetWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbasksToDependencies();
	}
	return OverlayWidgetController;
}

void AFPSHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	//UUserWidget* Widget = CreateWidget<UUserWidget>( GetWorld(), OverlayWidgetClass);
	//OverlayWidget = Cast<UFPSWidget>(Widget);
	
	OverlayWidget = CreateWidget<UFPSUserWidget>(GetWorld(),OverlayWidgetClass);
	UOverlayWidgetController* WidgetController = GetWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}
