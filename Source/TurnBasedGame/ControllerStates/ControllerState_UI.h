// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ControllerStateBase.h"
#include "../UI/InputWidget.h"

#include "GameplayTagContainer.h"
#include "ControllerState_UI.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UControllerState_UI : public UControllerStateBase
{
    GENERATED_BODY()

    UPROPERTY()
        UInputWidget* mWidget;

    DECLARE_EVENT_OneParam(UControllerState_UI, FActionSelected, FGameplayTag)
    FActionSelected ActionSelectedEvent;

    DECLARE_EVENT(UControllerState_UI, FCancelSelected)
    FCancelSelected CancelSelectedEvent;

public:

    void Setup(UInputWidget* mWidget);

    void OnMoveUp() override;
    void OnMoveDown() override;
    void OnMoveLeft() override;
    void OnMoveRight() override;
    void OnAction() override;
    void OnCancel() override;

    FActionSelected& OnActionSelected() // to subscribe to
    {
        return ActionSelectedEvent;
    }

    FCancelSelected& OnCancelSelected() // to subscribe to
    {
        return CancelSelectedEvent;
    }
};
