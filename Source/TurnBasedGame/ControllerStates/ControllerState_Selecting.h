// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerStateBase.h"
#include "../Helpers/GridPosition.h"
#include "../GameGrid.h"
#include "ControllerState_Selecting.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UControllerState_Selecting : public UControllerStateBase
{
    GENERATED_BODY()

protected:
        FGridPosition mStartingTile;

    UPROPERTY()
        AGridTile* mCurrentTile;
    UPROPERTY()
        AGameGrid* mGrid;

    // events -- theorically, a singlecast and dynamiccast could work...but i want to try events 
    DECLARE_EVENT_OneParam(UControllerState_Selecting, FSelectionChanged, AGridTile*)
    FSelectionChanged TileChangedEvent; 

    DECLARE_EVENT(UControllerState_Selecting, FSelectionError)
    FSelectionError SelectionErrorEvent; 

    DECLARE_EVENT(UControllerState_Selecting, FCharacterSelected)
    FCharacterSelected OnCharacterSelectEvent; 

    DECLARE_EVENT_OneParam(UControllerState_Selecting, FEnemyCharacterSelected, AGameCharacter*)
    FEnemyCharacterSelected OnEnemyCharacterSelectEvent;

    DECLARE_EVENT(UControllerState_Selecting, FEmptyTileSelected)
    FEmptyTileSelected OnEmtpyTileSelectEvent;

public:
    void Setup(AGridTile * startingTile, AGameGrid* grid);

    void OnMoveUp() override;
    void OnMoveDown() override;
    void OnMoveLeft() override;
    void OnMoveRight() override;
    void OnAction() override;

    FSelectionChanged& OnTileChanged() // to subscribe to
    {
        return TileChangedEvent;
    }

    FSelectionError& OnSelectionError() // to subscribe to
    {
        return SelectionErrorEvent;
    }

    FCharacterSelected& OnCharacterSelected() // to subscribe to
    {
        return OnCharacterSelectEvent;
    }

    FEnemyCharacterSelected& OnEnemyCharacterSelected() // to subscribe to
    {
        return OnEnemyCharacterSelectEvent;
    }

    FEmptyTileSelected& OnEmptyTileSelected() // to subscribe to
    {
        return OnEmtpyTileSelectEvent;
    }

    void RevertToTile(AGridTile* tile);

protected:
    AGridTile* GetCurrentTile() const;

private:
    virtual bool IsValidPosition(AGridTile* newTile);
    virtual void AssignNextTile(int xChanges, int yChanges);
};
