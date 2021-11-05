// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerState_Selecting.h"
#include "../GridTile.h"
#include "ControllerState_Movement.generated.h"

/**
 *  
 */
UCLASS()
class TURNBASEDGAME_API UControllerState_Movement : public UControllerState_Selecting
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
};
