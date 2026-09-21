// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Character/FPSMainCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AFPSPlayerController::AFPSPlayerController()
{
	bReplicates = true;
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputMappingContext);
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(InputMappingContext,0);
	}
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AFPSPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&AFPSPlayerController::Look);
	EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered,this,&AFPSPlayerController::Jump);
	EnhancedInputComponent->BindAction(StartFireAction,ETriggerEvent::Started,this,&AFPSPlayerController::StartFire1);
	EnhancedInputComponent->BindAction(StartFireAction,ETriggerEvent::Completed,this,&AFPSPlayerController::StopFire);
	EnhancedInputComponent->BindAction(ReloadAction,ETriggerEvent::Triggered,this,&AFPSPlayerController::Reload);
	EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Triggered,this,&AFPSPlayerController::Sprint);
	EnhancedInputComponent->BindAction(AimAction,ETriggerEvent::Started,this,&AFPSPlayerController::StartAim);
	EnhancedInputComponent->BindAction(AimAction,ETriggerEvent::Completed,this,&AFPSPlayerController::StopAim);
	EnhancedInputComponent->BindAction(SwitchFireModeAction,ETriggerEvent::Started,this,&AFPSPlayerController::SwitchFireMode);
}

void AFPSPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	ACharacter* FPSCharacter = GetCharacter();
	if (FPSCharacter)
	{
		if (SprintAlphaValue>=0.4f && MovementVector.Y>0.f)//仅前进可以收枪冲刺
		{
			FPSCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.f;
		}
		else FPSCharacter->GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
	SprintAlphaValue/=2;
	const FRotator ControllRotation	 = GetControlRotation();
	const FRotator YawRotation(0,ControllRotation.Yaw,0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(ControllRotation).GetUnitAxis(EAxis::Y);

	if (APawn* FPSPawn = GetPawn())
	{
		FPSPawn->AddMovementInput(ForwardDirection,MovementVector.Y);
		FPSPawn->AddMovementInput(RightDirection,MovementVector.X);
	}
}

void AFPSPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookDirection = InputActionValue.Get<FVector2D>();
	if (ACharacter* FPSCharacter = GetCharacter())
	{
		FPSCharacter->AddControllerYawInput(LookDirection.X);
		FPSCharacter->AddControllerPitchInput(-LookDirection.Y);
	}
}

void AFPSPlayerController::StartFire1(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		AFPSMainCharacter* FPSCharacter = CastChecked<AFPSMainCharacter>(ControlledCharacter);
		FPSCharacter->StartFire();
	}
}

void AFPSPlayerController::StopFire(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		AFPSMainCharacter* FPSCharacter = CastChecked<AFPSMainCharacter>(ControlledCharacter);
		FPSCharacter->StopFire();
	}
}

void AFPSPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->Jump();
	}
}

void AFPSPlayerController::Lean(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		
	}
}

void AFPSPlayerController::StartAim(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		AFPSMainCharacter* FPSCharacter = CastChecked<AFPSMainCharacter>(ControlledCharacter);
		FPSCharacter->StartAim();
	}
}

void AFPSPlayerController::StopAim(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlleredCharacter = GetCharacter())
	{
		AFPSMainCharacter* FPSCharacter = CastChecked<AFPSMainCharacter>(ControlleredCharacter);
		FPSCharacter->StopAim();
	}
}

void AFPSPlayerController::Reload(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlleredCharacter = GetCharacter())
	{
		AFPSMainCharacter* FPSCharacter = CastChecked<AFPSMainCharacter>(ControlleredCharacter);
		FPSCharacter->Reload();
	}
}

void AFPSPlayerController::Sprint(const FInputActionValue& InputActionValue)
{
	SprintAlphaValue = 1;
}

void AFPSPlayerController::SwitchFireMode(const FInputActionValue& InputActionValue)
{
	if (AFPSMainCharacter* FPSCharacter = Cast<AFPSMainCharacter>(GetCharacter()))
	{
		FPSCharacter->SwitchFireMode();
	}
}
