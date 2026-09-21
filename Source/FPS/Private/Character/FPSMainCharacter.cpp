// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSMainCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/FPSAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gameplay/FPSGameModeBase.h"
#include "Player/FPSPlayerState.h"
#include "UI/HUD/FPSHUD.h"
#include "Weapon/FPSWeaponBase.h"
AFPSMainCharacter::AFPSMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCameraComponent");
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->bUsePawnControlRotation = true;

	FirstPersonArm = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonArm");
	FirstPersonArm->SetupAttachment(Camera);
}

void AFPSMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Camera) return;
	const float TargetFOV = bIsAiming ? AimingFOV : DefaultFOV;
	const float NewFOV = FMath::FInterpTo(Camera->FieldOfView,TargetFOV,DeltaTime,AimFOVInterpSpeed);
	Camera->SetFieldOfView(NewFOV);
	
}

void AFPSMainCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (Camera)
	{
		OutLocation = Camera->GetComponentLocation();
		OutRotation = Camera->GetComponentRotation();
	}
	else
	{
		Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	}
	
}

void AFPSMainCharacter::BindCallbacksToDependencies()
{
	UFPSAttributeSet* FPSAttributeSet = Cast<UFPSAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FPSAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
			if (!HasAuthority()) return;
			if (Data.NewValue<=0&&!bIsDead) 
			{
				bIsDead = true;
				OnDeath();
			}
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FPSAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
}

void AFPSMainCharacter::StartFire()
{
	if (Weapon) Weapon->StartFire(this);
}

void AFPSMainCharacter::StopFire()
{
	if (Weapon) Weapon->StopFire();
}

void AFPSMainCharacter::Reload()
{
	if (Weapon) Weapon->DoReload();
}

void AFPSMainCharacter::StartAim()
{
	bIsAiming = true;
}

void AFPSMainCharacter::StopAim()
{
	bIsAiming = false;
}

void AFPSMainCharacter::SwitchFireMode()
{
	if (Weapon)
	{
		Weapon->SwitchFireMode();
	}
}

void AFPSMainCharacter::SpawnWeapon()
{
	if (WeaponClass)
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = this;
		spawnParameters.Instigator = this;
		Weapon = GetWorld()->SpawnActor<AFPSWeaponBase>(WeaponClass,spawnParameters);
	}
	if (Weapon)	
	{
		Weapon->AttachToComponent(FirstPersonArm,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("SOCKET_Weapon"));
		Weapon->FirstPersonArm = FirstPersonArm;
	}
}

void AFPSMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SpawnWeapon();
	InitAbilityActorInfo();
}

void AFPSMainCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AFPSMainCharacter::InitAbilityActorInfo()
{
	AFPSPlayerState* FPSPlayerState = Cast<AFPSPlayerState>(GetPlayerState());
	if (!FPSPlayerState)
	{
		return;
	}
	FPSPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(FPSPlayerState,this);
	AbilitySystemComponent = FPSPlayerState->GetAbilitySystemComponent();
	AttributeSet = FPSPlayerState->GetAttributeSet();
	if (APlayerController* FPSPlayerController = Cast<APlayerController>(GetController()))
	{
		if (AFPSHUD* FPSHUD = Cast<AFPSHUD>(FPSPlayerController->GetHUD()))
		{
			FPSHUD->InitOverlay(FPSPlayerController,FPSPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	InitDefaultAttribute();
	if (UFPSAttributeSet* AS = Cast<UFPSAttributeSet>(AttributeSet))
	{
		OnHealthInitialized.Broadcast(AS->GetHealth());
		OnMaxHealthInitialized.Broadcast(AS->GetMaxHealth());
	}
	BindCallbacksToDependencies();
}

void AFPSMainCharacter::OnDeath()
{
	if (AFPSGameModeBase* GM = Cast<AFPSGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->PlayerDied();
	}
}
