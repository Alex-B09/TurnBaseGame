// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameCharacter.h"

#include "Subsystems/WorldSubsystem.h"
#include "TurnSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeTurn, int, TurnNo, bool, IsPlayerTurn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartTurn, bool, IsPlayerTurn);

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
    void ProcessFinishCharacter(AGameCharacter* character);
    bool IsCharacterAvailable(AGameCharacter* character) const;
    

    // TODO change those names...they are terrible
    UPROPERTY(BlueprintAssignable)
        FStartTurn EndTurnEvent; // logic to end the turn -- this will be call before OnNewTurn and OnStartTurn
    UPROPERTY(BlueprintAssignable) 
        FChangeTurn NewTurnEvent; // this will be call after OnEndTurn -- this is meant for the UI
    UPROPERTY(BlueprintCallable)
        FStartTurn StartTurnEvent; // This will be call last and should be broadcasted from the ui when all the visual are done
};
