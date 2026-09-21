// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FPSEnemyBase.h"

#include "AbilitySystem/FPSAbilitySystemComponent.h"
#include "AbilitySystem/FPSAttributeSet.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/HitResult.h"
#include "Gameplay/FPSGameModeBase.h"


AFPSEnemyBase::AFPSEnemyBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFPSAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UFPSAttributeSet>("EnemyAttributeSet");
}

void AFPSEnemyBase::BeginPlay()
{
	// 重点：执行 Blueprint BeginPlay / 父类 BeginPlay 链
	Super::BeginPlay();

	InitAbilityActorInfo();
	
	DebugAttributeSet = AttributeSet;


}
void AFPSEnemyBase::InitAbilityActorInfo()
{
    // 防止 ASC 本身为空导致直接崩溃
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogTemp, Error,
            TEXT("[ERROR] AbilitySystemComponent == nullptr!"));

        return;
    }

    if (!AttributeSet)
    {
        UE_LOG(LogTemp, Error,
            TEXT("[ERROR] AttributeSet == nullptr BEFORE ASC Init!"));
    }

    // GAS ActorInfo 初始化
    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    // 初始化默认 Attribute
    InitDefaultAttribute();

    UFPSAttributeSet* AS = Cast<UFPSAttributeSet>(AttributeSet);

    OnHealthInitialized.Broadcast(AS->GetHealth());
    OnMaxHealthInitialized.Broadcast(AS->GetMaxHealth());

    BindCallbacksToDependencies();

}

void AFPSEnemyBase::BindCallbacksToDependencies()
{
	UFPSAttributeSet* FPSAttributeSet = Cast<UFPSAttributeSet>(AttributeSet);
	UE_LOG(LogTemp, Warning, TEXT("Bind: AttributeSet 地址 = %p"), AttributeSet.Get());
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

void AFPSEnemyBase::DoAttackDamage()
{
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = StartLoc + GetActorForwardVector() * 200.f;

	const float SphereRadius = 45.f;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);

	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SphereSweep), false);
	QueryParams.AddIgnoredActor(this);
	
	TArray<FHitResult> Hit;
	bool bIsHit = GetWorld()->SweepMultiByObjectType(Hit,StartLoc,EndLoc,FQuat::Identity,CollisionParams,CollisionShape,QueryParams);
	if (bIsHit)
	{
		for (const FHitResult HitResult : Hit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)	
			{
				IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(HitResult.GetActor());
				if(HitActor->IsA<AFPSEnemyBase>())
				{
					continue;
				}
				if (Interface&&AttackDamageEffect)
				{
					if (UAbilitySystemComponent* TargetASC = Interface->GetAbilitySystemComponent())
					{
						FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
						FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttackDamageEffect,1,ContextHandle);
						AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);
					}
				}
				break;
			}
		}
	}
}

void AFPSEnemyBase::OnDeath()
{
	if (AFPSGameModeBase* GM = Cast<AFPSGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->EnemyDied(this);
	}
}


