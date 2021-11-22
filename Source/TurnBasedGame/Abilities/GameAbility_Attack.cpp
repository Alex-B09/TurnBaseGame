// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility_Attack.h"
#include "../GameplaySubsystem.h"
#include "../TurnBasedGamePlayerController.h"

#include "DamageEffect.h"
#include "../Helpers/TagsConst.h"
#include "GameplayTagsManager.h"
//#include ""

// in C++ just to test how to do it
UGameAbility_Attack::UGameAbility_Attack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    mGameplayEventsToWaitTo.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(TagConst::EVENT_WEAPONHIT));
}

void UGameAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
        {
            // should never happen

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
        auto tiles = gameplaySubsystem->GetAvailableAttackTiles(avatar);
        gameplaySubsystem->HighlighGridForAttack(avatar);
        // Set controller state
        auto controller = Cast<ATurnBasedGamePlayerController>(world->GetFirstPlayerController());
        if (!controller)
        {
            UE_LOG(LogTemp, Log, TEXT("UGameAbility_Attack::ActivateAbility - invalid controller"));
            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
            return;
        }


        // wait for input -- register to controller 
        controller->OnCharacterSelect.AddDynamic(this, &UGameAbility_Attack::SelectCharacter);
        controller->OnCancelled.AddDynamic(this, &UGameAbility_Attack::AttackCancelled);

        // set mode
        controller->SetAttackMode();

        // add tag?

        // dont end ability here
    }
}

void UGameAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Cleanup();
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

    gameplaySubsystem->HideGridHighlight();

    auto effect = mAttackEffect->GetDefaultObject<UGameplayEffect>();
    CurrentActorInfo->AbilitySystemComponent->ApplyGameplayEffectToTarget(effect, character->GetAbilitySystemComponent(), 0.f);

    mGameplayEventHandle = CurrentActorInfo
                            ->AbilitySystemComponent
                            ->AddGameplayEventTagContainerDelegate(mGameplayEventsToWaitTo, 
                                                                   FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, 
                                                                                                                                &UGameAbility_Attack::HandleEvent));
    // i'm a bit confuse here.
    //  even though the function is private (HandleEvent), Unreal sees no problem calling it... (and not even an ufunction)
    //  but, meh...i'm learning things
    //  thanks ARPG demo

    HandleVisual();
}

void UGameAbility_Attack::AttackCancelled()
{
    // cancel state

    auto controller = Cast<ATurnBasedGamePlayerController>(GetWorld()->GetFirstPlayerController());

    if (!controller)
    {
        UE_LOG(LogTemp, Log, TEXT("UGameAbility_Attack::TileSelected - invalid inputs"));
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }

    // TODO - return to menu
    // must need something like a stack of controller state.
    // this is not going to be easy...
    
}

void UGameAbility_Attack::ApplyDamageEvent()
{
    UE_LOG(LogTemp, Log, TEXT("UGameAbility_Attack::ApplyDamageEvent - WOOOOOOOOOOOOOOOOOOT"));
    
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
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameAbility_Attack::HandleEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
    // this is a boilerplate function to call the right function
    ApplyDamageEvent();
}

void UGameAbility_Attack::Cleanup()
{
    auto controller = Cast<ATurnBasedGamePlayerController>(GetWorld()->GetFirstPlayerController());
    controller->OnCharacterSelect.RemoveDynamic(this, &UGameAbility_Attack::SelectCharacter);
    CurrentActorInfo->AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(mGameplayEventsToWaitTo, mGameplayEventHandle);
}
