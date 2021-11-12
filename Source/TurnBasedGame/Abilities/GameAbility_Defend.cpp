// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility_Defend.h"

// in C++ just to test how to do it
UGameAbility_Defend::UGameAbility_Defend()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameAbility_Defend::Setup(TSubclassOf<UGameplayEffect> defenseEffect)
{
    mDefenseEffect = defenseEffect;
}

void UGameAbility_Defend::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
        {
            // should never happen

            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
            return;
        }

        // the thing

        auto effect = mDefenseEffect->GetDefaultObject<UGameplayEffect>();
        ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, effect, 1, 1);

        // wait for new turn -- TODO create new subsystem?
    }
}
