// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerState_Selecting.h"

#include "../GameplaySubsystem.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void UControllerState_Selecting::Setup(AGridTile* startingTile, AGameGrid * grid)
{
    mCurrentTile = startingTile;
    mGrid = grid;
}

void UControllerState_Selecting::OnMoveUp()
{
    AssignNextTile(0, 1);
}

void UControllerState_Selecting::OnMoveDown()
{
    AssignNextTile(0, -1);
}

void UControllerState_Selecting::OnMoveLeft()
{
    AssignNextTile(-1, 0);
}

void UControllerState_Selecting::OnMoveRight()
{
    AssignNextTile(1, 0);
}

void UControllerState_Selecting::OnAction()
{
    auto world = mGrid->GetWorld();
    auto gameplaySubsystem = world->GetSubsystem<UGameplaySubsystem>();
    auto tileStatus = gameplaySubsystem->GetTileOccupationStatus(mCurrentTile);
    auto character = gameplaySubsystem->GetCharacter(mCurrentTile);

    if (tileStatus == ETileOccupationState::Empty)
    {
        UE_LOG(LogTemp, Log, TEXT("Tile Empty"));
        OnEmptyTileSelected().Broadcast();
    }
    else if (tileStatus == ETileOccupationState::IsCharacterEnemy)
    {
        UE_LOG(LogTemp, Log, TEXT("Tile enemy"));
        OnEnemyCharacterSelectEvent.Broadcast(character);
    }
    else if (tileStatus == ETileOccupationState::IsCharacterPlayer)
    {
        UE_LOG(LogTemp, Log, TEXT("Tile character"));
        OnCharacterSelectEvent.Broadcast();
    }
}

bool UControllerState_Selecting::IsValidPosition(AGridTile * newTile)
{
    return newTile != nullptr;
}

void UControllerState_Selecting::AssignNextTile(int xChanges, int yChanges)
{
    // its a bit slow to always search in the grid...but meh
    FGridPosition pos = mGrid->GetTilePosition(mCurrentTile);
    pos.mPosX += xChanges;
    pos.mPosY += yChanges;
    auto newTile = mGrid->GetTile(pos);

    if (IsValidPosition(newTile))
    {
        TileChangedEvent.Broadcast(newTile);
        mCurrentTile = newTile;
    }
    else
    {
        SelectionErrorEvent.Broadcast();
    }
}

void UControllerState_Selecting::RevertToTile(AGridTile* tile)
{
    if (!tile)
    {
        UE_LOG(LogTemp, Log, TEXT("UControllerState_Selecting::RevertToTile - invalid tile"));
        return;
    }

    mCurrentTile = tile;
}
