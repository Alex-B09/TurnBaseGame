// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility_ReceiveDamage.h"

#include "../GameCharacter.h"

#include "../Helpers/TagsConst.h"
#include "GameplayTagsManager.h"


UGameAbility_ReceiveDamage::UGameAbility_ReceiveDamage(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    FAbilityTriggerData triggerData;
    triggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    triggerData.TriggerTag = UGameplayTagsManager::Get().RequestGameplayTag(TagConst::EVENT_DMGRECEIVED);


    AbilityTriggers.Add(triggerData);
}

void UGameAbility_ReceiveDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    UE_LOG(LogTemp, Log, TEXT("---------------- in UGameAbility_ReceiveDamage::ActivateAbility"));

    auto character = Cast<AGameCharacter>(ActorInfo->AvatarActor);
    if (!character)
    {
        UE_LOG(LogTemp, Log, TEXT("UGameAbility_ReceiveDamage::ActivateAbility -- invalid character - not supposed to happen"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    mDamageToApply = static_cast<int>(character->GetAttributes()->DamageToApply.GetCurrentValue());
    if (character->GetAttributes()->Health.GetCurrentValue() <= 0.f)
    {
        HandleVisualDeath();
    }
    else
    {
        HandleVisualDamage();
    }

    
}

void UGameAbility_ReceiveDamage::VisualDone()
{
    // technicaly, the only one subscribe to this is the GA_attack 
    FGameplayEventData dmgEvent;
    dmgEvent.Instigator = CurrentActorInfo->AvatarActor.Get();
    SendGameplayEvent(UGameplayTagsManager::Get().RequestGameplayTag(TagConst::EVENT_DAMAGEFINISH), dmgEvent);

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
