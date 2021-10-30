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

protected:
    DECLARE_EVENT(UControllerStateBase, FStateChanged)
    FStateChanged StateChangedEvent; // event for the states to go to the next one

public:
    FStateChanged& OnStateChange() // to subscribe to
    { 
        return StateChangedEvent;
    }

    virtual void StartState() {}
    virtual void OnMoveUp() {}
    virtual void OnMoveDown() {}
    virtual void OnMoveRight() {}
    virtual void OnMoveLeft() {}
    virtual void OnAction() {}
    virtual void OnCancel() {}

private:
};
