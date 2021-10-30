// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GameGrid.h"
#include "InputWidget.h"
#include "ControllerStates/ControllerStateBase.h"
#include "ControllerStates/ControllerActionState.h"

#include "TurnBasedGamePlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerGridSelect, AGridTile*, Tile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerCancelled);

UCLASS()
class ATurnBasedGamePlayerController : public APlayerController
{
    GENERATED_BODY()

private:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Grid"))
        AGameGrid* mGrid;

    UPROPERTY()
        EControllerActionState mState = EControllerActionState::Selecting;

    UPROPERTY()
        AGameCharacter* mSelectedCharacter;

    // technicaly, i dont need the uproperty here...but i had so many problems in the past...
    //  i'm not taking any chances
    UPROPERTY()
        FGridPosition mPosition;

    UPROPERTY()
        UInputWidget* mWidget;

    UPROPERTY()
        UControllerStateBase* mControllerState;
public:
    ATurnBasedGamePlayerController();

    void SetMovementMode();

    // this exists for the "wait for" ability tasks
    UPROPERTY()
        FControllerGridSelect OnTileSelect;
    UPROPERTY()
        FControllerCancelled OnCancelled;

    UFUNCTION(BlueprintCallable, BlueprintPure)
        AGameCharacter* GetCharacter() const;

protected:
    // Begin PlayerController interface
    virtual void BeginPlay() override;
    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // inputs mapping
    void OnMoveUp();
    void OnMoveDown();
    void OnMoveRight();
    void OnMoveLeft();
    void OnAction();
    void OnCancel();

    UFUNCTION(BlueprintImplementableEvent)
        void ShowActionMenu();

    UFUNCTION(BlueprintCallable)
        void SetUIWidget(UInputWidget* widget);

    UFUNCTION(BlueprintImplementableEvent)
        void OnWatchTile(AGridTile* toWatch);

    UFUNCTION(BlueprintImplementableEvent)
        void OnMovementError();

private:
    void WatchCurrentTile();
    AGridTile* GetCurrentTile() const;

    void SetupFristState();
    void OnStateTileChanged();

    void OnCharacterSelected();

};
