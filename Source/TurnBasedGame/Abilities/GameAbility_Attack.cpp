// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility_Attack.h"
#include "../GameplaySubsystem.h"
#include "../TurnBasedGamePlayerController.h"
#include "../GridManipulatorSubsystem.h"

#include "Abilities/Tasks/AbilityTask.h"
#include "DamageEffect.h"
#include "../Helpers/TagsConst.h"
#include "GameplayTagsManager.h"

//#include ""

// in C++ just to test how to do it
UGameAbility_Attack::UGameAbility_Attack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    mGameplayEventsToWaitTo.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(TagConst::EVENT_WEAPONHIT));
    mGameplayTargetEvents.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(TagConst::EVENT_DAMAGEFINISH));
}

void UGameAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
        {
            // should never happen
            UE_LOG(LogTemp, Log, TEXT("UGameAbility_Attack::ActivateAbility - cannot commit ability - should not happen"));
            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
            return;
        }

        auto avatar = Cast<AGameCharacter>(GetAvatarActorFromActorInfo());
        if (!avatar && !(avatar->GetWorld()) && !(avatar->GetWorld()->GetSubsystem<UGameplaySubsystem>()))
        {
			UE_LOG(LogTemp, Log, TEXT("UGameAbility_Attack::ActivateAbility - invalid input"));
            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
            return;
        }

        mSourceCharacter = avatar;

        auto world = avatar->GetWorld();
        // get tiles
        auto gameplaySubsystem = world->GetSubsystem<UGameplaySubsystem>();
        auto controller = Cast<ATurnBasedGamePlayerController>(world->GetFirstPlayerController());
        if (!gameplaySubsystem || !controller)
        {
            UE_LOG(LogTemp, Log, TEXT("UGameAbility_Attack::ActivateAbility - invalid controller or subsystem"));
            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
            return;
        }

        //gameplaySubsystem->ChangeToAttackMode();

        // wait for input -- register to controller 
        controller->OnCharacterSelect.AddDynamic(this, &UGameAbility_Attack::SelectCharacter);
        controller->OnCancelled.AddDynamic(this, &UGameAbility_Attack::AttackCancelled);

        // dont end ability here
    }
}

void UGameAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Cleanup();

    // hide attack
    auto avatar = Cast<AGameCharacter>(GetAvatarActorFromActorInfo());
    auto world = avatar->GetWorld();
    auto gridSubsystem = world->GetSubsystem<UGridManipulatorSubsystem>();
    
    gridSubsystem->RevertAttackState(); // TODO - check if this is better here or in the controller

    if (!bWasCancelled)
    {
        DelayEnd(); // just pad a bit
    }

    // this seems to be important
    // is there an "EndAbility_implementation" somewhere? Haven't found one yet
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (!bWasCancelled)
    {
        NotifyControllerEndAction();
    }
}

bool UGameAbility_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    return true;
}

void UGameAbility_Attack::SelectCharacter(AGameCharacter* character)
{
    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    auto controller = Cast<ATurnBasedGamePlayerController>(GetWorld()->GetFirstPlayerController());

    if (!gameplaySubsystem || !controller || !character)
    {
        UE_LOG(LogTemp, Log, TEXT("UGameAbility_Attack::SelectCharacter - invalid inputs"));
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }

    if (character == GetAvatarActorFromActorInfo())
    {
        // not the right selection
        controller->OnSelectionError();
        return;
    }

    mTargetCharacter = character;

    // keep it clean
    controller->OnCharacterSelect.RemoveDynamic(this, &UGameAbility_Attack::SelectCharacter);

    //gameplaySubsystem->HideGridHighlight();

    auto effect = mAttackEffect->GetDefaultObject<UGameplayEffect>();
    CurrentActorInfo->AbilitySystemComponent->ApplyGameplayEffectToTarget(effect, character->GetAbilitySystemComponent(), 0.f);

    mGameplayEventHandle = CurrentActorInfo
                            ->AbilitySystemComponent
                            ->AddGameplayEventTagContainerDelegate(mGameplayEventsToWaitTo, 
                                                                   FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, 
                                                                                                                                &UGameAbility_Attack::HandleEvent));
    // i'm a bit confuse here.
    //  even though the function is not an ufunction (HandleEvent), Unreal sees no problem calling it...the CreateUOBject is amazing
    //  but, meh...i'm learning things
    //  thanks ARPG demo


    // interesting functionnality:
    //          if the FGameplayTagContainer (first argument) is empty, it will register to all events
    mGameplayTargetEventHandle = mTargetCharacter
                                    ->GetAbilitySystemComponent()
                                    ->AddGameplayEventTagContainerDelegate(mGameplayTargetEvents,
                                                                           FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this,
                                                                                                                                        &UGameAbility_Attack::HandleEventTarget));

    HandleVisual();
}

void UGameAbility_Attack::AttackCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameAbility_Attack::ApplyDamageEvent()
{
    UE_LOG(LogTemp, Log, TEXT("UGameAbility_Attack::ApplyDamageEvent"));
    
    // clean up
    CurrentActorInfo->AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(mGameplayEventsToWaitTo, mGameplayEventHandle);

    FGameplayEventData dmgEvent;
    dmgEvent.EventTag = UGameplayTagsManager::Get().RequestGameplayTag(TagConst::EVENT_DMGRECEIVED);
    dmgEvent.Instigator = mSourceCharacter;
    dmgEvent.Target = mTargetCharacter;
    
    // send to the target the event to complete the cycle
    mTargetCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(dmgEvent.EventTag, &dmgEvent);
}

void UGameAbility_Attack::VisualDone()
{
    mVisualDone = true;

    if (mVisualDone && mTargetFinishedLogic)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UGameAbility_Attack::HandleEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
    // this is a boilerplate function to call the right function
    ApplyDamageEvent();
}

void UGameAbility_Attack::HandleEventTarget(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
    // should only be called for "Receive damage done"
    UE_LOG(LogTemp, Log, TEXT("---------------- in UGameAbility_Attack::HandleEventTarget -- %s"), *(EventTag.GetTagName()).ToString());

    mTargetFinishedLogic = true;

    if (mVisualDone && mTargetFinishedLogic)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UGameAbility_Attack::Cleanup()
{
    if (auto controller = Cast<ATurnBasedGamePlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        controller->OnCharacterSelect.RemoveDynamic(this, &UGameAbility_Attack::SelectCharacter);
        controller->OnCancelled.RemoveDynamic(this, &UGameAbility_Attack::AttackCancelled);
    }

    CurrentActorInfo->AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(mGameplayEventsToWaitTo, mGameplayEventHandle);

    if (mTargetCharacter)
    {
        mTargetCharacter->GetAbilitySystemComponent()->RemoveGameplayEventTagContainerDelegate(mGameplayTargetEvents, mGameplayTargetEventHandle);
    }
}
