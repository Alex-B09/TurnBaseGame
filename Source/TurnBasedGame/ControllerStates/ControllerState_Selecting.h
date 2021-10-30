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

private:
    FGridPosition* mPosition; // can't uproperty that one...hope this wont screw up everything later...

    UPROPERTY()
        AGameGrid* mGrid;

    // events -- theorically, a singlecast could work...but i want to try events 
    DECLARE_EVENT(UControllerState_Selecting, FSelectionChanged)
    FSelectionChanged TileChangedEvent; 

    DECLARE_EVENT(UControllerState_Selecting, FSelectionError)
    FSelectionError SelectionErrorEvent; 

    DECLARE_EVENT(UControllerState_Selecting, FCharacterSelected)
    FCharacterSelected OnCharacterSelectEvent; 

public:
    void Setup(FGridPosition* position, AGameGrid* grid);

    void OnMoveUp() override;
    void OnMoveDown() override;
    void OnMoveLeft() override;
    void OnMoveRight() override;
    void OnAction() override;
    void OnCancel() override;


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
};
