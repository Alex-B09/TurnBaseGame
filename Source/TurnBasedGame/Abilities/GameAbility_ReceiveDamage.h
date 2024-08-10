// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilityBase.h"
#include "GameAbility_ReceiveDamage.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UGameAbility_ReceiveDamage : public UGameAbilityBase
{
    GENERATED_BODY()

private:
    bool isDying;
    
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
        int mDamageToApply;

public:
    UGameAbility_ReceiveDamage(const FObjectInitializer& ObjectInitializer);

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;



    UFUNCTION(BlueprintImplementableEvent)
        void HandleVisualDeath();
    UFUNCTION(BlueprintImplementableEvent)
        void HandleVisualDamage();

protected:
    UFUNCTION(BlueprintCallable)
        void VisualDone();
};
