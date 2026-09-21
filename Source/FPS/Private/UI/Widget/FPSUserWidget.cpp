// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/FPSUserWidget.h"

void UFPSUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
