// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"

#include "InputWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UInputWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputUp();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputDown();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputLeft();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputRight();
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputAction(FGameplayTag &tag);
    UFUNCTION(BlueprintImplementableEvent, Category = "Inputs")
        void OnInputCancel();
    UFUNCTION(BlueprintImplementableEvent, Category = "Logic")
        void CloseUI();
    UFUNCTION(BlueprintImplementableEvent, Category = "Logic")
        void HideUI();
    UFUNCTION(BlueprintImplementableEvent, Category = "Logic")
        void UnHideUI();


    // C++ friendly helper function
    FGameplayTag OnInputAction();
};
