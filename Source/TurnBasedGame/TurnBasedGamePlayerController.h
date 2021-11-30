// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GameGrid.h"
#include "UI/InputWidget.h"
#include "ControllerStates/ControllerStateBase.h"
#include "ControllerStates/ControllerActionState.h"

#include "TurnBasedGamePlayerController.generated.h"

// i have tried both event and dynamic multicast
//  Dynamic multicast need the uproperty reflection system to work properly
//  Event is the old C++14 way of doing things
//  
//  I have a slight preference for dynamic multicast since it plays well with BP

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerGridSelect, AGridTile*, Tile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerCharacterSelect, AGameCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControllerCancelled);

UCLASS()
class ATurnBasedGamePlayerController : public APlayerController
{
    GENERATED_BODY()

private:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Grid"))
        AGameGrid* mGrid;

    UPROPERTY()
        AGameCharacter* mSelectedCharacter;

    // technicaly, i dont need the uproperty here...but i had so many problems in the past...
    //  i'm not taking any chances
    UPROPERTY()
        AGridTile * mCurrentTile;

    UPROPERTY()
        UInputWidget* mWidget;

    UPROPERTY()
        TArray<UControllerStateBase*> mStateStack;

public:
    ATurnBasedGamePlayerController();

    UFUNCTION(BlueprintCallable)
        void SetMovementMode();
    void SetAttackMode();
    void FinishActionCharacter();

    // this exists for the "wait for" ability tasks
    UPROPERTY()
        FControllerGridSelect OnTileSelect;
    UPROPERTY()
        FControllerCharacterSelect OnCharacterSelect; // only work for enemy for now
    UPROPERTY(BlueprintAssignable)
        FControllerCancelled OnCancelled;
    UPROPERTY(BlueprintAssignable)
        FControllerGridSelect OnTileChanged;


    UFUNCTION(BlueprintCallable, BlueprintPure)
        AGameCharacter* GetCharacter() const;

    UFUNCTION(BlueprintImplementableEvent)
        void OnSelectionError();

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

    UControllerStateBase* GetDefaultState();
    void OnCharacterSelected();
    void ProcessUIAction(FGameplayTag tag);

    void SwitchCurrentTile(AGridTile* newTile);

    void ChangeToActionMenu();

    UControllerStateBase* GetState();
    void RemoveState(UControllerStateBase* toRemove);
};
