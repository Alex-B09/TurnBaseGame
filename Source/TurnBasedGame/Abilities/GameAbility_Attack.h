// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilityBase.h"

#include "../GridTile.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameAbility_Attack.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UGameAbility_Attack : public UGameAbilityBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
        TSubclassOf<UGameplayEffect> mAttackEffect;

    UPROPERTY()
        AGameCharacter* mSourceCharacter;
    UPROPERTY()
        AGameCharacter* mTargetCharacter;

    UPROPERTY()
        FGameplayTagContainer mGameplayEventsToWaitTo;
    FDelegateHandle mGameplayEventHandle;


    UPROPERTY()
        FGameplayTagContainer mGameplayTargetEvents;
    FDelegateHandle mGameplayTargetEventHandle;

    bool mVisualDone = false;
    bool mTargetFinishedLogic = false;

public:
    UGameAbility_Attack();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

    UFUNCTION(BlueprintImplementableEvent)
        void HandleVisual();


    // for documentation
    //virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    // need to be public for dynamic cast
    UFUNCTION()
        void SelectCharacter(AGameCharacter* character);

    UFUNCTION()
        void AttackCancelled();

    UFUNCTION()
        void ApplyDamageEvent();

    

protected:
    UFUNCTION(BlueprintCallable)
        void VisualDone();

private:
    void HandleEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);
    void HandleEventTarget(FGameplayTag EventTag, const FGameplayEventData* Payload);

    void Cleanup();
};
