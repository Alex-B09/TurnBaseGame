// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI_InputResponder.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUI_InputResponder : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASEDGAME_API IUI_InputResponder
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Input Response")
        void OnInputUp();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Input Response")
        void OnInputDown();

    UFUNCTION(BlueprintImplementableEvent, Category = "Input Response")
        void OnInputLeft();

    UFUNCTION(BlueprintImplementableEvent, Category = "Input Response")
        void OnInputRight();

    UFUNCTION(BlueprintImplementableEvent, Category = "Input Response")
        void OnInputCancel();

    UFUNCTION(BlueprintImplementableEvent, Category = "Input Response")
        void OnInputAction();

};
