// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Selecting.h"
#include "../GridTile.h"
#include "ControllerState_Attack.generated.h"

/**
 *  
 */
UCLASS()
class TURNBASEDGAME_API UControllerState_Attack : public UControllerState_Selecting
{
    GENERATED_BODY()
        
    UPROPERTY()
        TArray<AGridTile*> mAvailableTiles; // list of available movement tiles
public:
    void Setup(AGridTile * startingPosition, 
               AGameGrid* grid,
               TArray<AGridTile*> availableTiles);

private:
    bool IsValidPosition(AGridTile* tile) override;
    void AssignNextTile(int xChanges, int yChanges) override;
};
