// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ControllerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UControllerStateBase : public UObject
{
    GENERATED_BODY()


public:
    DECLARE_EVENT(UControllerStateBase, FSelectionChanged)
    FSelectionChanged& OnTileChanged() // to subscribe
    { 
        // theorically, a singlecast could work...but i want to try events
        return TileChangedEvent;
    }

    virtual void StartState() {}
    virtual void OnMoveUp() {}
    virtual void OnMoveDown() {}
    virtual void OnMoveRight() {}
    virtual void OnMoveLeft() {}
    virtual void OnAction() {}
    virtual void OnCancel() {}

private:
    /** Broadcasts whenever the layer changes */
    FSelectionChanged TileChangedEvent;
};
