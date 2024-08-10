// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnSubsystem.h"
#include "GameplaySubsystem.h" // i dont like the fact that i need to use this here
                               // i should maybe create an other layer where the data will be. Another subsystem??? that seems meta a bit
#include "GameplayTagsManager.h"

#include "Helpers/TagsConst.h"

void UTurnSubsystem::GoToNextTeamTurn()
{
    UE_LOG(LogTemp, Log, TEXT("UTurnSubsystem::GoToNextTeamTurn - NEW TURN!!!"));

    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    if (!gameplaySubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("UTurnSubsystem::GoToNextTeamTurn - invalid gameplay subsystem"));
        return;
    }

    mIsPlayerTurn = !mIsPlayerTurn;

    if (mIsPlayerTurn)
    {
        ++mTurnNumber;
        mRemainingTeamCharacters = gameplaySubsystem->GetPlayerCharacters();
    }
    else
    {
        mRemainingTeamCharacters = gameplaySubsystem->GetEnemyCharacters();
    }

    SendNewTurnEventToCharacters();

    EndTurnEvent.Broadcast(mIsPlayerTurn);
    BeforeStartTurnEvent.Broadcast(mTurnNumber, mIsPlayerTurn);
}


bool UTurnSubsystem::IsCharacterAvailable(AGameCharacter * character) const
{
    return mRemainingTeamCharacters.Contains(character);
}


void UTurnSubsystem::ProcessFinishCharacter(AGameCharacter* character)
{
    if (!character)
    {
        UE_LOG(LogTemp, Log, TEXT("UTurnSubsystem::ProcessFinishCharacter - invalid character"));
        return;
    }

    if (!IsCharacterAvailable(character))
    {
        UE_LOG(LogTemp, Log, TEXT("UTurnSubsystem::ProcessFinishCharacter - character is not available"));
        return;
    }

    mRemainingTeamCharacters.Remove(character);

    if (mRemainingTeamCharacters.Num() <= 0)
    {
        // set for next tick to clean up the callstack
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTurnSubsystem::GoToNextTeamTurn);
    }
}


void UTurnSubsystem::SendNewTurnEventToCharacters()
{
    FGameplayEventData endTurnEvent;
    endTurnEvent.EventTag = UGameplayTagsManager::Get().RequestGameplayTag(TagConst::DURATION_NEWTURN);

    // send to the target the event to complete the cycle of ability that ends at the beginning of the new turn
    for (auto character : mRemainingTeamCharacters)
    {
        // This could potentially cancels langering effects that needs to be disabled at the beginning of the player's turn
        character->GetAbilitySystemComponent()->HandleGameplayEvent(endTurnEvent.EventTag, &endTurnEvent);
    }
}
