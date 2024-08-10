// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerState_Movement.h"

void UControllerState_Movement::Setup(AGridTile* startingPosition,
                                      AGameGrid* grid, 
                                      TArray<AGridTile*> availableTiles)
{
    
    mGrid = grid;
    mCurrentTile = startingPosition;
    mAvailableTiles = availableTiles;
}

bool UControllerState_Movement::IsValidPosition(AGridTile* tile)
{
    return mAvailableTiles.Contains(tile);
}
