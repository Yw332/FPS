// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FPSWeaponBase.h"

#include "AbilitySystemComponent.h"
#include "Character/FPSCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/FPSEnemyBase.h"
#include "Sound/SoundBase.h"


// Sets default values
AFPSWeaponBase::AFPSWeaponBase()
{

	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	RootComponent = WeaponMesh;

	MagazineCapacity = 30;
	FireRate = 0.1f;
	Damage = 20.f;
	TraceRange = 10000.f;
	FireMode = EWeaponFireMode::FullAuto;

	CurrentAmmo = 0;     
	ReServeAmmo = 0;
	bIsReloading = false;
	LastFireTime = -999.f;  
	CurrentPawn = nullptr;
	bWeaponCanFire = true;

	RecoilPitchRange = -0.3f;
	
}

FVector AFPSWeaponBase::GetMuzzleLocation()
{
	TArray<USceneComponent*> Components;
	GetComponents(Components);
	for (auto i : Components)
	{
		if (i && i->ComponentHasTag("Muzzle"))
		{
			return i->GetComponentLocation();
		}
	}
	return WeaponMesh->GetSocketLocation(TEXT("SOCKET_Muzzle"));
}

FText AFPSWeaponBase::GetFireModeText(EWeaponFireMode Mode)
{
	switch (Mode)
	{
	case EWeaponFireMode::SemiAuto:
		return NSLOCTEXT("Weapon", "FireMode_SemiAuto", "SemiAuto");
	case EWeaponFireMode::FullAuto:
		return NSLOCTEXT("Weapon", "FireMode_FullAuto", "FullAuto");
	default:
		return NSLOCTEXT("Weapon", "FireMode_Unknown", "Unknown");
	}
}

void AFPSWeaponBase::SwitchFireMode()
{
	if (bIsReloading) return;
	if (FireMode == EWeaponFireMode::FullAuto) FireMode = EWeaponFireMode::SemiAuto;
	else FireMode = EWeaponFireMode::FullAuto;
	OnFireModeChanged.Broadcast(FireMode);
}

void AFPSWeaponBase::Fire(APawn* OwenPawn)
{
	//前置条件判断
	UWorld* World = GetWorld();
	if (bIsReloading || !OwenPawn || CurrentAmmo <= 0 || !World || !bWeaponCanFire) return;
	
	//相机射线检测 获取Aimpoint
	FVector CameraLoc;
	FRotator CameraRot;
	OwenPawn->GetActorEyesViewPoint(CameraLoc, CameraRot);
	FVector TraceEnd = CameraLoc + CameraRot.Vector() * TraceRange;
	FHitResult AimHit;
	GetWorld()->LineTraceSingleByChannel(AimHit,CameraLoc,TraceEnd,ECC_Visibility);
	FVector AimPoint = AimHit.bBlockingHit?AimHit.Location:TraceEnd;
	
	FVector MuzzleLoc = GetMuzzleLocation();
	FVector FireDir = (AimPoint - MuzzleLoc).GetSafeNormal();
	
	
	FHitResult BulletHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwenPawn);
	
	//开火音效
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
		World,
		FireSound,
		MuzzleLoc
		);
	}
	//开火特效：
	if (MuzzleFlashEffects)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World,
		MuzzleFlashEffects,
		MuzzleLoc,
		FireDir.Rotation()
		);
	}
	//手臂开火蒙太奇播放
	if (UAnimInstance* ArmAnimInstance = FirstPersonArm->GetAnimInstance())
	{
		if (ArmAnimInstance)
		{
			ArmAnimInstance->Montage_Play(ArmFireMontage);
		}
		
	}
	//武器开火蒙太奇播放
	if (UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance())
	{
		if (WeaponFireMontage)
		{
			WeaponAnimInstance->Montage_Play(WeaponFireMontage);
		}
		
	}
	
	//子弹射线检测
	bool bIsHit = World->LineTraceSingleByChannel(BulletHit,MuzzleLoc,MuzzleLoc+ FireDir * TraceRange,ECC_GameTraceChannel1,Params);
	//DrawDebugLine(World, MuzzleLoc, MuzzleLoc+ FireDir * TraceRange, FColor::Yellow, false, 10.0f, 0, 1.0f);
	if (bIsHit)
	{
		AActor* HitActor = BulletHit.GetActor();
		if (HitActor)
		{
			//UGameplayStatics::ApplyDamage(BulletHit.GetActor(), Damage, OwenPawn->GetController(),this,nullptr);
			AFPSEnemyBase* HitEnemy = Cast<AFPSEnemyBase>(HitActor);

			//敌人受击特效：
			if (HitEnemy&&EnemyHitEffects)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					World,
					EnemyHitEffects,
					BulletHit.Location,
					BulletHit.ImpactNormal.Rotation()
				);
			}
			if (HitEnemy&&ShotBodySound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					World,
					ShotBodySound,
					BulletHit.Location
				);
			}
			//弹孔特效：
			if (BulletHoleDecalMaterial)
			{
				UGameplayStatics::SpawnDecalAtLocation(
					World,
					BulletHoleDecalMaterial,
					FVector(3.f,3.f,3.f),
					BulletHit.Location,
					BulletHit.ImpactNormal.Rotation(),
					10.f
				);
			}
			if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(HitActor))
			{
				UAbilitySystemComponent* TargetASC = ASCInterface->GetAbilitySystemComponent();
				if (TargetASC && DamageEffect)
				{
					FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
					FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect,1,ContextHandle);
					TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}			
		}
	}
	
	CurrentAmmo--;
	OnCurrentAmmoChanged.Broadcast(CurrentAmmo);
	LastFireTime = World->GetTimeSeconds();

}



void AFPSWeaponBase::DoReload()
{
	if (bIsReloading) return;
	if (ReServeAmmo <= 0 || CurrentAmmo >= MagazineCapacity)
	{
		return;
	}
	bIsReloading = true;

	float ReloadDuration = 1.2f;
	//手臂换弹蒙太奇
	if (UAnimInstance* AnimInstance = FirstPersonArm->GetAnimInstance())
	{
		if (ArmReloadMontage)
		{
			ReloadDuration = AnimInstance->Montage_Play(ArmReloadMontage);
		}
		
	}
	//武器换弹蒙太奇
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (WeaponAnimInstance)
	{
		if (WeaponReloadMontage)
		{
			WeaponAnimInstance->Montage_Play(WeaponReloadMontage);
		}
		
	}
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AFPSWeaponBase::EndReload, ReloadDuration, false);
}


void AFPSWeaponBase::EndReload()
{
	if (MagazineCapacity-CurrentAmmo>=ReServeAmmo)
	{
		CurrentAmmo += ReServeAmmo;
		ReServeAmmo = 0;
	}
	else
	{
		ReServeAmmo -= (MagazineCapacity-CurrentAmmo);
		CurrentAmmo = MagazineCapacity;
	}
	OnCurrentAmmoChanged.Broadcast(CurrentAmmo);
	OnReserveAmmoChanged.Broadcast(ReServeAmmo);
	bIsReloading = false;
}

void AFPSWeaponBase::StartFire(APawn* OwenPawn)
{
	//前置条件判断
	UWorld* World = GetWorld();
	if (!World) return;
	if (bIsReloading || !OwenPawn || !bWeaponCanFire) return;
	if (CurrentAmmo<=0)
	{
		if (FireEmptySound)
		{
			UGameplayStatics::PlaySoundAtLocation(
			World,
			FireEmptySound,
			GetMuzzleLocation()
			);
		}
		return;
	}
	if (GetWorld()->GetTimeSeconds() - LastFireTime < FireRate) return;
	CurrentPawn = OwenPawn;
	Fire(CurrentPawn);
	if (FireMode == EWeaponFireMode::FullAuto)
	{
		GetWorldTimerManager().SetTimer(
			AutoFireTimerHandle,
			this,
			&AFPSWeaponBase::AutoFire,
			FireRate,
			true);
	}
}

void AFPSWeaponBase::AutoFire()
{
	if (bIsReloading)
	{
		GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
		return;
	}
	if (CurrentAmmo <= 0 && bWeaponCanFire)
	{
		UWorld* World = GetWorld();
		if (FireEmptySound&&World)
		{
			UGameplayStatics::PlaySoundAtLocation(
			World,
			FireEmptySound,
			GetMuzzleLocation()
			);
		}
	}
	
	Fire(CurrentPawn);
	ACharacter* Character = Cast<ACharacter>(CurrentPawn);
	if (bWeaponCanFire&&Character)
	{
		RecoilYawRange = FMath::RandRange(-0.1,0.1);
		Character->AddControllerPitchInput(RecoilPitchRange);
		Character->AddControllerYawInput(RecoilYawRange);
	}
}

void AFPSWeaponBase::StopFire()
{
	GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
}

// Called when the game starts or when spawned
void AFPSWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MagazineCapacity;
	ReServeAmmo = 999;
	LastFireTime = -999.f;
	
	OnCurrentAmmoChanged.Broadcast(CurrentAmmo);
	OnReserveAmmoChanged.Broadcast(ReServeAmmo);
	OnFireModeChanged.Broadcast(FireMode);
}

void AFPSWeaponBase::Destroyed()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	Super::Destroyed();
}


