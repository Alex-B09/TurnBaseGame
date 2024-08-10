// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ControllerActionState.h"

#include "ControllerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UControllerStateBase : public UObject
{
    GENERATED_BODY()

public:
    virtual void StartState() {}
    virtual void OnMoveUp() {}
    virtual void OnMoveDown() {}
    virtual void OnMoveRight() {}
    virtual void OnMoveLeft() {}
    virtual void OnAction() {}

    virtual void OnCancel() 
    {
        CancelSelectedEvent.Broadcast();
    }

protected:
    DECLARE_EVENT(UControllerStateBase, FCancelSelected)
    FCancelSelected CancelSelectedEvent;

    DECLARE_EVENT(UControllerStateBase, FStateResumed)
    FStateResumed StateResumeEvent;

public:
    FCancelSelected& OnCancelSelected() // to subscribe to
    {
        return CancelSelectedEvent;
    }

    FStateResumed& OnStateResumed() // to subscribe to
    {
        return StateResumeEvent;
    }

    void ResumeState()
    {
        StateResumeEvent.Broadcast();
    }

private:
};
