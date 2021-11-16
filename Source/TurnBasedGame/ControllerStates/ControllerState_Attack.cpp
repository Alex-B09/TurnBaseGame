// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerState_Attack.h"
#include "../GameplaySubsystem.h"

void UControllerState_Attack::Setup(AGridTile* startingPosition,
                                    AGameGrid* grid, 
                                    TArray<AGridTile*> availableTiles)
{
    if (!grid || !startingPosition || availableTiles.Num() <= 0)
    {
		UE_LOG(LogTemp, Log, TEXT("UControllerState_Attack::Setup - invalid arguments"));
        return;
    }

    mGrid = grid;
    mCurrentTile = startingPosition;
    mAvailableTiles = availableTiles;
}

bool UControllerState_Attack::IsValidPosition(AGridTile* tile)
{
    return mAvailableTiles.Contains(tile);
}

void UControllerState_Attack::AssignNextTile(int xChanges, int yChanges)
{
    auto gameplaySubsystem = mCurrentTile->GetWorld()->GetSubsystem<UGameplaySubsystem>();

    // its a bit slow to always search in the grid...but meh
    FGridPosition pos = mGrid->GetTilePosition(mCurrentTile);
    pos.mPosX += xChanges;
    pos.mPosY += yChanges;
    auto newTile = mGrid->GetTile(pos);


    if (IsValidPosition(newTile) && gameplaySubsystem->GetCharacter(newTile) != nullptr)
    {
        TileChangedEvent.Broadcast(newTile);
    }
    else
    {
        SelectionErrorEvent.Broadcast();
    }
}
