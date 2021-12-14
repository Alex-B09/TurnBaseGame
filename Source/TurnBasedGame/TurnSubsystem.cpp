// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnSubsystem.h"
#include "GameplaySubsystem.h" // i dont like the fact that i need to use this here
                               // i should maybe create an other layer where the data will be. Another subsystem??? that seems meta a bit
#include "GameplayTagsManager.h"

#include "Helpers/TagsConst.h"

void UTurnSubsystem::GoToNextTeamTurn()
{
    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    if (!gameplaySubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("UTurnSubsystem::GoToNextTeamTurn - invalid gameplay subsystem"));
        return;
    }

    if (mIsPlayerTurn)
    {
        mRemainingTeamCharacters = gameplaySubsystem->GetEnemyCharacters();
    }
    else
    {
        ++mTurnNumber;
        mRemainingTeamCharacters = gameplaySubsystem->GetPlayerCharacters();
    }
    mIsPlayerTurn = !mIsPlayerTurn;


    // TODO - move this to controller?
    FGameplayEventData dmgEvent;
    dmgEvent.EventTag = UGameplayTagsManager::Get().RequestGameplayTag(TagConst::DURATION_NEWTURN);

    // send to the target the event to complete the cycle of ability that ends at the begenning of the new turn
    for (auto character : mRemainingTeamCharacters)
    {
        character->GetAbilitySystemComponent()->HandleGameplayEvent(dmgEvent.EventTag, &dmgEvent);
    }

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
        GoToNextTeamTurn();
    }
}

