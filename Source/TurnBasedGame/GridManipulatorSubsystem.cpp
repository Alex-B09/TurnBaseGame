// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManipulatorSubsystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UGridManipulatorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    // that did not work
    // the subsystem is initialize before the actors are added to the world

    // this is left here as documentation

    //TArray<AActor*> foundActors;
    //UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameGrid::StaticClass(), foundActors);

    //if (foundActors.Num() > 0)
    //{
    //    mGrid = Cast<AGameGrid>(foundActors[0]);
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Log, TEXT("UGridManipulatorSubsystem::Initialize - did not find grid"));
    //}
}


void UGridManipulatorSubsystem::SetGrid(AGameGrid* grid)
{
    mGrid = grid;
}


void UGridManipulatorSubsystem::HighlighTilesForMovement(TArray<AGridTile*> movementTiles) const
{
    for (auto tile : movementTiles)
    {
        tile->SetToMovementHighlight();
    }
}


void UGridManipulatorSubsystem::SelectForMovement(AGridTile* selectedMovementTile, AGridTile* oldMovementTile) const
{
    // desactivate old attack tile
    if (oldMovementTile)
    {
        oldMovementTile->RemoveLastState();
    }

    selectedMovementTile->SetToSelection();
}


void UGridManipulatorSubsystem::HighlighTilesForAttack(TArray<AGridTile*> attackTiles) const
{
    for (auto tile : attackTiles)
    {
        tile->SetToAttackHighlight();
    }
}


void UGridManipulatorSubsystem::SelectTileForAttack(AGridTile* selectedAttackTile, AGridTile* oldAttackTile) const
{
    // desactivate old attack tile
    if (oldAttackTile)
    {
        oldAttackTile->RemoveLastState();
    }

    selectedAttackTile->SetToAttackSelection();
}


void UGridManipulatorSubsystem::RevertAttackState()
{
    if (!mGrid)
    {
        UE_LOG(LogTemp, Log, TEXT("UGridManipulatorSubsystem::RevertAttackState - invalid grid"));
        return;
    }

    auto tiles = mGrid->GetTiles();

    for (auto tile : tiles)
    {
        if (tile->GetState() == ETileState::HighlightForAttack)
        {
            tile->RemoveLastState();
        }
        else if (tile->GetState() == ETileState::SelectedForAttack)
        {
            // TODO do something better
            tile->RemoveAllState();
            tile->RemoveAllState();
        }
    }
}


void UGridManipulatorSubsystem::SelectTile(AGridTile* selectedTile, AGridTile* oldSelectedTile) const
{
    // desactivate old attack tile
    if (oldSelectedTile)
    {
        oldSelectedTile->RemoveLastState();
    }

    selectedTile->SetToSelection();
}


void UGridManipulatorSubsystem::RemoveTilesOverlays()
{
    if (!mGrid)
    {
        UE_LOG(LogTemp, Log, TEXT("UGridManipulatorSubsystem::RemoveTilesOverlays - invalid grid"));
        return;
    }

    auto tiles = mGrid->GetTiles();

    for (auto tile : tiles)
    {
        tile->RemoveAllState();
    }
}

void UGridManipulatorSubsystem::HighlightCharacter(AGridTile* selectedTile)
{
    if (!selectedTile)
    {
        UE_LOG(LogTemp, Log, TEXT("UGridManipulatorSubsystem::HighlightCharacter - invalid tile"));
        return;
    }

    if (selectedTile->GetState() != ETileState::SelectedCharacter)
    {
        selectedTile->SetToCharacterSelected();
    }
}

void UGridManipulatorSubsystem::RemoveCharacterHighlight(AGridTile* selectedTile)
{
    if (!selectedTile)
    {
        UE_LOG(LogTemp, Log, TEXT("UGridManipulatorSubsystem::RemoveCharacterHighlight - invalid tile"));
        return;
    }

    if (selectedTile->GetState() == ETileState::SelectedCharacter)
    {
        selectedTile->RemoveLastState();
    }
}
