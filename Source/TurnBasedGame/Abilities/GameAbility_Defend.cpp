// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility_Defend.h"
#include "../Helpers/TagsConst.h"
#include "GameplayTagsManager.h"
#include "AbilitySystemComponent.h"

// in C++ just to test how to do it
UGameAbility_Defend::UGameAbility_Defend()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

//void UGameAbility_Defend::Setup(TSubclassOf<UGameplayEffect> defenseEffect)
//{
//    mDefenseEffect = defenseEffect;
//}

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
        HandleVisual();

        FGameplayTagContainer gameplayEventsToWaitTo;
        gameplayEventsToWaitTo.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(TagConst::DURATION_NEWTURN));
        auto delegateHandle = CurrentActorInfo
            ->AbilitySystemComponent
            ->AddGameplayEventTagContainerDelegate(gameplayEventsToWaitTo,
                                                   FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UGameAbility_Defend::EndTurnEventReceive));

    }
}

void UGameAbility_Defend::VisualDone()
{
    mIsVisualDone = true;
    NotifyControllerEndAction();
}

void UGameAbility_Defend::EndTurnEventReceive(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
    UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------sdsdss----------------"));

    CurrentActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(mDefenseEffect, CurrentActorInfo->AbilitySystemComponent.Get());

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
