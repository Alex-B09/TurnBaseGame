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
    mStartingTile = startingPosition;
    mAvailableTiles = availableTiles;
}

bool UControllerState_Attack::IsValidPosition(AGridTile* tile)
{
    return mAvailableTiles.Contains(tile);
}

void UControllerState_Attack::AssignNextTile(int xChanges, int yChanges)
{
    auto gameplaySubsystem = mStartingTile->GetWorld()->GetSubsystem<UGameplaySubsystem>();

    // its a bit slow to always search in the grid...but meh
    FGridPosition pos = mGrid->GetTilePosition(mStartingTile);
    pos.mPosX += xChanges;
    pos.mPosY += yChanges;
    mCurrentTile = mGrid->GetTile(pos); // this is needed for the parent
    
    if (IsValidPosition(mCurrentTile) && gameplaySubsystem->GetCharacter(mCurrentTile) != nullptr)
    {
        TileChangedEvent.Broadcast(mCurrentTile);
    }
    else
    {
        SelectionErrorEvent.Broadcast();
    }
}

void UControllerState_Attack::OnAction()
{
    auto gameplaySubsystem = mStartingTile->GetWorld()->GetSubsystem<UGameplaySubsystem>();
    
    //check if there is a target
    if (auto targetCharacter = gameplaySubsystem->GetCharacter(mCurrentTile))
    {
        OnEnemyCharacterSelected().Broadcast(targetCharacter);
    }
    else
    {
        OnEmptyTileSelected().Broadcast();
    }


}

// TODO - there need to be something in the near future to have a better tile selection
//        by this i mean that the state_attack should look at the available tiles, the possible target and have custom input controls
//        to, maybe, have different range/patterns
//        for example, an attack pattern could be:
//              X
//              X
//            XXOXX
//              X
//              X
//        where O is the attacker and X are possible target
//        or even:
//              X
//              -
//            X-O-X
//              -
//              X
//        or any other kind of patterns

