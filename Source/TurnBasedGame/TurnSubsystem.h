// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameCharacter.h"

#include "Subsystems/WorldSubsystem.h"
#include "TurnSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeTurn, int, TurnNo, bool, IsPlayerTurn);

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UTurnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
private:
    UPROPERTY()
        int mTurnNumber = 0;

    UPROPERTY()
        bool mIsPlayerTurn = false; // this is set to false to create a trick. See GoToNextTeamTurn def to have more details
    
    UPROPERTY()
        TArray<AGameCharacter*> mRemainingTeamCharacters;

public:
    void GoToNextTeamTurn();
    void RemoveCharacter(AGameCharacter* character);
    bool IsCharacterAvailable(AGameCharacter* character) const;
    
    UPROPERTY(BlueprintAssignable)
        FChangeTurn OnNewTurn;
};
