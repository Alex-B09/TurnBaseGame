// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility_Attack.h"
#include "../GameplaySubsystem.h"
#include "../TurnBasedGamePlayerController.h"

// in C++ just to test how to do it
UGameAbility_Attack::UGameAbility_Attack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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
        // remove highlights

        // attack
        // 
        // wait for cue
        // wait for death


        // wait for new turn -- TODO create new subsystem?
        // add tag?

        //EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }
}

//void UGameAbility_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
//{
//
//}

//void UGameAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
//{
//
//}

bool UGameAbility_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    return true;
}
