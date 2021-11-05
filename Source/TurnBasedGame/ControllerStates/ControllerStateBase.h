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
    virtual void OnCancel() {}

private:
};
