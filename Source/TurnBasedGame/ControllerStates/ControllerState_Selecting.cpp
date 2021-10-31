// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerState_Selecting.h"

#include "../GameplaySubsystem.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void UControllerState_Selecting::Setup(FGridPosition* position, AGameGrid * grid)
{
    mPosition = position;
    mGrid = grid;
    // highlight character

    auto world = mGrid->GetWorld();
    auto gameplaySubsystem = world->GetSubsystem<UGameplaySubsystem>();

    //gameplaySubsystem->GetCharacter()
    //gameplaySubsystem->HighlighGridForCharacter();
}

void UControllerState_Selecting::OnMoveUp()
{
    int width = mGrid->GetDepth();

    if (mPosition->mPosY + 1 < width)
    {
        mPosition->mPosY += 1;
        TileChangedEvent.Broadcast();
    }
    else
    {
        SelectionErrorEvent.Broadcast();
    }
}

void UControllerState_Selecting::OnMoveDown()
{
    int width = mGrid->GetDepth();

    if (mPosition->mPosY - 1 >= 0)
    {
        mPosition->mPosY -= 1;
        TileChangedEvent.Broadcast();
    }
    else
    {
        SelectionErrorEvent.Broadcast();
    }
}

void UControllerState_Selecting::OnMoveLeft()
{
    if (mPosition->mPosX - 1 >= 0)
    {
        mPosition->mPosX -= 1;
        TileChangedEvent.Broadcast();
    }
    else
    {
        SelectionErrorEvent.Broadcast();
    }
}

void UControllerState_Selecting::OnMoveRight()
{
    int depth = mGrid->GetWidth();
    if (mPosition->mPosX + 1 < depth)
    {
        mPosition->mPosX += 1;
        TileChangedEvent.Broadcast();
    }
    else
    {
        SelectionErrorEvent.Broadcast();
    }
}

void UControllerState_Selecting::OnAction()
{
    auto world = mGrid->GetWorld();
    auto gameplaySubsystem = world->GetSubsystem<UGameplaySubsystem>();

    auto tile = mGrid->GetTile(*mPosition);
    auto tileStatus = gameplaySubsystem->GetTileStatus(tile);

    if (tileStatus == EGridTileState::Empty)
    {
        UE_LOG(LogTemp, Log, TEXT("Tile Empty"));
    }
    else if (tileStatus == EGridTileState::IsCharacterEnemy)
    {
        UE_LOG(LogTemp, Log, TEXT("Tile enemy"));
    }
    else if (tileStatus == EGridTileState::IsCharacterPlayer)
    {
        UE_LOG(LogTemp, Log, TEXT("Tile character"));

        OnCharacterSelectEvent.Broadcast();

        UE_LOG(LogTemp, Log, TEXT("Selected"));
    }
}

void UControllerState_Selecting::OnCancel()
{
    // go back to old state
}
