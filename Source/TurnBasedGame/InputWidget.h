// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UInputWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputUp();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputDown();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputLeft();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputRight();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputAction();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputCancel();
};
