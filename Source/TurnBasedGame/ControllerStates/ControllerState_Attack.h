// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Selecting.h"
#include "../GridTile.h"
#include "../GameplaySubsystem.h"
#include "ControllerState_Attack.generated.h"

/**
 *  
 */
UCLASS()
class TURNBASEDGAME_API UControllerState_Attack : public UControllerState_Selecting
{
    GENERATED_BODY()
        
    
    TArray<FAttackTarget> mAttackTargets; // list of available movement tiles
    FAttackTarget mCurrentTarget;
    
    UPROPERTY()
        AGridTile* mStartingTile;


public:
    void Setup(AGridTile * startingPosition, 
               AGameGrid* grid,
               TArray<FAttackTarget> availableTargets);

    void SetupFirstTarget();

    void OnMoveUp() override;
    void OnMoveDown() override;
    void OnMoveLeft() override;
    void OnMoveRight() override;
    void OnAction() override;
private:
    bool IsValidPosition(AGridTile* tile) override;
    //void AssignNextTile(int xChanges, int yChanges) override;
    void NotifyNewTarget(FAttackTarget* target);
};
