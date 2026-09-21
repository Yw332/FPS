// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSWeaponBase.generated.h"

class UGameplayEffect;
class USkeletalMeshComponent;
class UNiagaraSystem;
class UMaterialInstance;
UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	SemiAuto UMETA(DisplayName = "SemiAuto"),
	FullAuto UMETA(DisplayName = "FullAuto")
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChangedSignature,int32,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireModeChangedSignature,EWeaponFireMode,NewValue);
UCLASS()
class FPS_API AFPSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFPSWeaponBase();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon|Attributes")
	int32 MagazineCapacity;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon|Attributes")
	int32 CurrentAmmo;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon|Attributes")
	int32 ReServeAmmo;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon|Attributes")
	float FireRate;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon|Attributes")
	float Damage;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon|Attributes")
	float TraceRange;
	UPROPERTY(EditAnywhere,Category="Weapon|Mesh")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY()
	USkeletalMeshComponent* FirstPersonArm;

	
	UPROPERTY(VisibleAnywhere,Category="Weapon|FireMode")
	EWeaponFireMode FireMode;
	
	UPROPERTY(EditAnywhere,Category="Weapon|Sound")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere,Category="Weapon|Sound")
	USoundBase* FireEmptySound;
	UPROPERTY(EditAnywhere,Category="Weapon|Sound")
	USoundBase* ShotBodySound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	bool bWeaponCanFire;
	

	UPROPERTY(EditAnywhere,Category="Weapon|Animation")
	UAnimMontage* ArmReloadMontage;
	UPROPERTY(EditAnywhere,Category="Weapon|Animation")
	UAnimMontage* ArmFireMontage;
	UPROPERTY(EditAnywhere,Category="Weapon|Animation")
	UAnimMontage* WeaponReloadMontage;
	UPROPERTY(EditAnywhere,Category="Weapon|Animation")
	UAnimMontage* WeaponFireMontage;

	UPROPERTY(EditAnywhere,Category="Weapon|VFX")
	TObjectPtr<UNiagaraSystem> MuzzleFlashEffects;
	UPROPERTY(EditAnywhere,Category="Weapon|VFX")
	TObjectPtr<UNiagaraSystem> EnemyHitEffects;
	UPROPERTY(EditAnywhere,Category="Weapon|VFX")
	TObjectPtr<UMaterialInstance> BulletHoleDecalMaterial;

	UPROPERTY(EditAnywhere,Category="Weapon|Recoil")
	float RecoilPitchRange;
	UPROPERTY(EditAnywhere,Category="Weapon|Recoil")
	float RecoilYawRange;

	//GE
	UPROPERTY(EditAnywhere,Category="Weapon|GameplayEffect")
	TSubclassOf<UGameplayEffect> DamageEffect;
	//GE

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChangedSignature OnCurrentAmmoChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChangedSignature OnReserveAmmoChanged;
	UPROPERTY(BlueprintAssignable)
	FOnFireModeChangedSignature OnFireModeChanged;
	
	UPROPERTY()
	bool bIsReloading = false;

	FVector GetMuzzleLocation();

	UFUNCTION(BlueprintPure)
	static FText GetFireModeText(EWeaponFireMode Mode);
	
	void SwitchFireMode();
	void DoReload();
	void EndReload();
	void StartFire(APawn* OwenPawn);
	void StopFire();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
private:
	float LastFireTime;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle AutoFireTimerHandle;
	UPROPERTY()
	TObjectPtr<APawn> CurrentPawn;
	void Fire(APawn* OwenPawn);
	void AutoFire();
};
