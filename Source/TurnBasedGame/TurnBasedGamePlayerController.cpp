// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedGamePlayerController.h"

// states
#include "ControllerStates/ControllerState_Selecting.h"
#include "ControllerStates/ControllerState_UI.h"
#include "ControllerStates/ControllerState_Movement.h"
#include "ControllerStates/ControllerState_Attack.h"

#include "Abilities/GameAbility_Movement.h"

#include "Helpers/TagsConst.h"
#include "Helpers/AbilityHelper.h"
#include "Helpers/ArrayHelper.h"
#include "GameplaySubsystem.h"
#include "GridManipulatorSubsystem.h"
#include "TurnSubsystem.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


ATurnBasedGamePlayerController::ATurnBasedGamePlayerController()
{
    bShowMouseCursor = false;
    //PrimaryActorTick.bCanEverTick = false; // TODO - test this
}


void ATurnBasedGamePlayerController::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameGrid::StaticClass(), foundActors);

    if (foundActors.Num() > 0)
    {
        if (auto grid = Cast<AGameGrid>(foundActors[0]))
        {
            mGrid = grid;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::BeginPlay - invalid grid"));
            return;
        }
    }

    auto starupTile = mGrid->GetTile(FGridPosition{ 0,0 });

    if (!starupTile)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::BeginPlay - invalid startup tile"));
        return;
    }

    SwitchCurrentTile(starupTile);
    mStateStack.Add(GetDefaultState());

    auto turnSubsystem = GetWorld()->GetSubsystem<UTurnSubsystem>();
    if (!turnSubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::BeginPlay - invalid turn subsystem"));
        return;
    }

    turnSubsystem->EndTurnEvent.AddDynamic(this, &ATurnBasedGamePlayerController::OnEndTurn);
    turnSubsystem->StartTurnEvent.AddDynamic(this, &ATurnBasedGamePlayerController::OnStartNewTurn);
}


void ATurnBasedGamePlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
}


void ATurnBasedGamePlayerController::SetupInputComponent()
{
    // set up gameplay key bindings
    Super::SetupInputComponent();

    InputComponent->BindAction("MoveUp", IE_Pressed, this, &ATurnBasedGamePlayerController::OnMoveUp);
    InputComponent->BindAction("MoveDown", IE_Pressed, this, &ATurnBasedGamePlayerController::OnMoveDown);
    InputComponent->BindAction("MoveRight", IE_Pressed, this, &ATurnBasedGamePlayerController::OnMoveRight);
    InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ATurnBasedGamePlayerController::OnMoveLeft);
    InputComponent->BindAction("Action", IE_Pressed, this, &ATurnBasedGamePlayerController::OnAction);
    InputComponent->BindAction("Cancel", IE_Pressed, this, &ATurnBasedGamePlayerController::OnCancel);
}


void ATurnBasedGamePlayerController::OnMoveUp()
{
    UE_LOG(LogTemp, Log, TEXT("Move up"));

    auto state = GetState();
    if (!state)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnMoveUp - state error"));
        return;
    }

    state->OnMoveUp();
}


void ATurnBasedGamePlayerController::OnMoveDown()
{
    UE_LOG(LogTemp, Log, TEXT("Move down"));

    auto state = GetState();
    if (!state)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnMoveDown - state error"));
        return;
    }

    state->OnMoveDown();
}


void ATurnBasedGamePlayerController::OnMoveLeft()
{
    UE_LOG(LogTemp, Log, TEXT("Move left"));

    auto state = GetState();
    if (!state)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnMoveLeft - state error"));
        return;
    }

    state->OnMoveLeft();
}


void ATurnBasedGamePlayerController::OnMoveRight()
{
    UE_LOG(LogTemp, Log, TEXT("Move Right"));

    auto state = GetState();
    if (!state)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnMoveRight - state error"));
        return;
    }

    state->OnMoveRight();
}


void ATurnBasedGamePlayerController::OnAction()
{
    UE_LOG(LogTemp, Log, TEXT("Action"));

    auto state = GetState();
    if (!state)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnAction - state error"));
        return;
    }

    state->OnAction();
}


void ATurnBasedGamePlayerController::OnCancel()
{
    UE_LOG(LogTemp, Log, TEXT("Cancel"));

    auto state = GetState();
    if (!state)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnCancel - state error"));
        return;
    }

    state->OnCancel();
}


void ATurnBasedGamePlayerController::WatchCurrentTile()
{
    if (!mGrid)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::WatchCurrentTile - invalid precondition"));
        return;
    }

	if (auto tile = GetCurrentTile())
	{
        OnWatchTile(tile);
        OnTileChanged.Broadcast(tile);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::WatchCurrentTile - invalid tile"));
	}

}


AGridTile* ATurnBasedGamePlayerController::GetCurrentTile() const
{
    return mCurrentTile;
}


AGameCharacter* ATurnBasedGamePlayerController::GetCharacter() const
{
    return mSelectedCharacter;
}


void ATurnBasedGamePlayerController::SetUIWidget(UInputWidget* widget)
{
    mWidget = widget;
}


UControllerStateBase* ATurnBasedGamePlayerController::GetDefaultState()
{
    // basic selection mode

    auto state = NewObject<UControllerState_Selecting>();
    state->Setup(mCurrentTile, mGrid);

    // AddDynamic does not work...for it to work
    //  meh...lambda works...
    state->OnTileChanged().AddLambda([=](AGridTile* newTile)
                                     {
                                         SwitchCurrentTile(newTile);
                                     });

    state->OnCharacterSelected().AddLambda([=]()
                                           {
                                               OnCharacterSelected();
                                           });

    return state;
}


void ATurnBasedGamePlayerController::SetMovementMode(TArray<AGridTile*> movementTiles)
{
    // TODO - check if already in movement mode
    auto currentState = GetState();

    if (Cast<UControllerState_Movement>(currentState))
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SetMovementMode - already in movement, nothing to do"));
        return;
    }

    if (!mSelectedCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SetMovementMode - invalid input"));
        return;
    }

    auto state = NewObject<UControllerState_Movement>();

    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    auto gridManipulator = GetGridManipulator();

    auto setupGridHighlights = [=]()
                                {
                                    gridManipulator->HighlighTilesForMovement(movementTiles);
                                    gridManipulator->SelectForMovement(mCurrentTile, nullptr);
                                };

    auto highlightOnlyCharacter = [=]()
    {
        gridManipulator->RemoveTilesOverlays();
        gridManipulator->SelectForMovement(mCurrentTile, nullptr);
        gridManipulator->HighlightCharacter(mCurrentTile);
    };

    setupGridHighlights();
    auto startingTile = mCurrentTile; // usefull to keep track of 

    state->Setup(mCurrentTile, mGrid, movementTiles);

    auto emptyTileEventLambda = [=]()
                                {
                                    // CAREFUL - C++ seems to have difficulties with lambda in lambda
                                    //         - the problem was that, the implicit "this" reference in the internal lambda was set to null for whatever reason 
                                    auto disableInputs = [=]()
                                                         {
                                                             //InputEnabled = false; // this does not work
                                                             DisableInput(this); // this does work...maybe its because of the "explicit" this
                                                             // https://www.nextptr.com/tutorial/ta1430524603/capture-this-in-lambda-expression-timeline-of-change 
                                                         };

                                    auto doneMovingLambda = [=] ()
                                                             {
                                                                 mSelectedCharacter->OnStartedMovement().Clear();
                                                                 mSelectedCharacter->OnFinishMovement().Clear();
                                                                 EnableInput(this);
                                                                 
                                                                 highlightOnlyCharacter();
                                                                 // I had a weird bug here that, when i used the gridManipulator, the controller died...
                                                                 // solved that by just putting the code in a lambda that was created externaly

                                                                 ChangeToActionMenu();
                                                             };

                                    mSelectedCharacter->OnStartedMovement().AddLambda(disableInputs);
                                    mSelectedCharacter->OnFinishMovement().AddLambda(doneMovingLambda);

                                    OnTileSelect.Broadcast(GetCurrentTile());
                                };

    auto selfSelectLambda = [=]()
                            {
                                // check if self
                                auto selectedTile = GetCurrentTile();
                                OnTileSelect.Broadcast(selectedTile);
        
                                auto character = gameplaySubsystem->GetCharacter(selectedTile);
                                if (character != mSelectedCharacter)
                                {
                                    return;
                                }

                                highlightOnlyCharacter();
                                ChangeToActionMenu();
                            };

    auto cancelEventLambda = [=]()
                             {
                                 if (mCurrentTile == startingTile) // cancel if same tile
                                 {
                                     OnCancelled.Broadcast(); // this will cancel the action and hide tiles

                                     gridManipulator->RemoveTilesOverlays();
                                     gridManipulator->SelectForMovement(mCurrentTile, nullptr);

                                     RemoveState(state);
                                 }
                                 else
                                 {
                                     SwitchCurrentTile(startingTile);
                                     state->RevertToTile(startingTile);
                                 }
                             };

    auto revertToLambda = [=]()
                            {
                                // just start the ability
                                // teleport character to start character
                                state->RevertToTile(startingTile);

                                gridManipulator->RemoveCharacterHighlight(mCurrentTile);

                                gameplaySubsystem->TeleportCharacter(mSelectedCharacter, startingTile);
                                SwitchCurrentTile(startingTile);
                                setupGridHighlights();

                                AbilityHelper::TryActivateAbilitybyHiearchicalClass<UGameAbility_Movement>(mSelectedCharacter->GetAbilitySystemComponent());
                            };

    state->OnEmptyTileSelected().AddLambda(emptyTileEventLambda);
    state->OnCancelSelected().AddLambda(cancelEventLambda);
    state->OnCharacterSelected().AddLambda(selfSelectLambda);
    state->OnStateResumed().AddLambda(revertToLambda);

    state->OnTileChanged().AddLambda([=](AGridTile* newTile)
                                     {
                                         SwitchCurrentTile(newTile);
                                     });

    mStateStack.Add(state);
}


void ATurnBasedGamePlayerController::SetAttackMode()
{
    // TODO - check if already in attack mode
    auto currentState = GetState();

    if (Cast<UControllerState_Attack>(currentState))
    {
        // this should never happen but better be safe then sorry
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SetAttackMode - already in attack, nothing to do"));
        return;
    }

    if (!mSelectedCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SetAttackMode - invalid input"));
        return;
    }
    
    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    auto attackTargets = gameplaySubsystem->GetAvailableAttackTarget(mSelectedCharacter);

    auto attackTiles = Select<AGridTile*>(attackTargets, [=](const FAttackTarget& data)
                                                         {
                                                             return data.mTile;
                                                         });
    
    auto characterTile = mCurrentTile;

    // highlight tiles
    auto gridManipulator = GetGridManipulator();
    gridManipulator->HighlighTilesForAttack(attackTiles);

    auto state = NewObject<UControllerState_Attack>();
    state->Setup(mCurrentTile, mGrid, attackTargets);

    state->OnEnemyCharacterSelected().AddLambda([=](AGameCharacter* enemyCharacter)
                                                {
                                                    OnCharacterSelect.Broadcast(enemyCharacter);
                                                });

    state->OnCancelSelected().AddLambda([=]()
                                        {
                                            // close ability
                                            OnCancelled.Broadcast(); // this will cancel the ability

                                            mCurrentTile = characterTile;
                                            RemoveState(state);
                                        });

    state->OnTileChanged().AddLambda([=](AGridTile * newTileSelection)
                                     {
                                         if (mCurrentTile == characterTile) // should only happen on the first try
                                         {
                                             gridManipulator->SelectTileForAttack(newTileSelection, nullptr);
                                         }
                                         else
                                         {
                                            gridManipulator->SelectTileForAttack(newTileSelection, mCurrentTile);
                                         }

                                         mCurrentTile = newTileSelection;
                                     });
    
    state->SetupFirstTarget();
    mStateStack.Add(state);
}


void ATurnBasedGamePlayerController::OnCharacterSelected()
{
    // change mode to ui
    // setup new state
    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    auto gridSubsystem = GetGridManipulator();

    if (!gameplaySubsystem || !gridSubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnCharacterSelected - invalid precondition"));
        return;
    }


    auto tile = GetCurrentTile();
    if (auto character = gameplaySubsystem->GetCharacter(tile))
    {
        mSelectedCharacter = character;
        tile->SetToCharacterSelected(); // TODO - switch to gridManipulator?
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnCharacterSelected - invalid character returned"));
        return;
    }

    auto movementTiles = gameplaySubsystem->GetAvailableMovementTiles(mSelectedCharacter);
    
    SetMovementMode(movementTiles);

    // activate ability
    AbilityHelper::TryActivateAbilitybyHiearchicalClass<UGameAbility_Movement>(mSelectedCharacter->GetAbilitySystemComponent());
}


void ATurnBasedGamePlayerController::ProcessUIAction(FGameplayTag tag)
{
    if (!mSelectedCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::ProcessUIAction - invalid character"));
        return;
    }

    if (tag.GetTagName() == TagConst::UI_DEFEND)
    {
        AbilityHelper::TryActivateAbilitybyHiearchicalClass<UGameAbility_Defend>(mSelectedCharacter->GetAbilitySystemComponent());
    }
    else if (tag.GetTagName() == TagConst::UI_ATTACK)
    {
        if (AbilityHelper::TryActivateAbilitybyHiearchicalClass<UGameAbility_Attack>(mSelectedCharacter->GetAbilitySystemComponent()))
        {
            SetAttackMode();
        }
    }
}


void ATurnBasedGamePlayerController::SwitchCurrentTile(AGridTile* newTile)
{
    auto gridManipulator = GetGridManipulator();
    if (!gridManipulator)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SwitchCurrentTile - invalid grid manipulator"));
        return;
    }

    gridManipulator->SelectTile(newTile, mCurrentTile);
    mCurrentTile = newTile;
    WatchCurrentTile();
}


void ATurnBasedGamePlayerController::ChangeToActionMenu()
{
    // go to ui mode  
    ShowActionMenu();

    if (!mWidget)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::ChangeToActionMenu - invalid widget"));
        return;
    }

    auto state = NewObject<UControllerState_UI>();
    state->Setup(mWidget);

    auto actionSelectedLambda = [=](FGameplayTag tag)
    {
        ProcessUIAction(tag);
        // close the ui
        if (!mWidget)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::ChangeToActionMenu - actionSelectedLambda - invalid widget"));
            return;
        }
        mWidget->HideUI();
    };

    auto cancelLambda = [=]()
    {
        // hide menu
        if (!mWidget)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::ChangeToActionMenu - cancelLambda - invalid widget"));
            return;
        }
        mWidget->CloseUI();
        RemoveState(state);
    };

    auto revertToLambda = [=]()
    {
        // show the ui
        if (!mWidget)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::ChangeToActionMenu - actionSelectedLambda - invalid widget"));
            return;
        }
        mWidget->UnHideUI();
    };

    // not pretty but i could not find how to add the function to the event
    //  the way i see it, event are great if you dont move away from C++
    //  i would prefer dynamic_multicast for more complex scenario
    state->OnActionSelected().AddLambda(actionSelectedLambda);
    state->OnStateResumed().AddLambda(revertToLambda);
    state->OnCancelSelected().AddLambda(cancelLambda);

    mStateStack.Add(state);
}


UControllerStateBase* ATurnBasedGamePlayerController::GetState()
{
    if (mStateStack.Num() <= 0)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::GetState - emtpy state"));
        
        // add default state
        auto defaultState = GetDefaultState();
        mStateStack.Add(defaultState);
    }
    return mStateStack.Last();
}


void ATurnBasedGamePlayerController::RemoveState(UControllerStateBase* toRemove)
{
    if (mStateStack.Num() <= 0)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::RemoveState - emtpy state"));
        return;
    }

    if (!toRemove || mStateStack.Last() != toRemove)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::RemoveState - invalid state"));
        return;
    }

    mStateStack.Pop(false);
    
    auto currentState = GetState(); // it does magic
    currentState->ResumeState();
}


void ATurnBasedGamePlayerController::FinishActionCharacter()
{
    auto turnSubsystem = GetWorld()->GetSubsystem<UTurnSubsystem>();
    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();

    if (!turnSubsystem || !gameplaySubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::FinishActionCharacter - invalid subsystem"));
        return;
    }

    if (!mSelectedCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::FinishActionCharacter - invalid character"));
        return;
    }

    // clean states
    mStateStack.Empty();
    GetState(); // it does magic and initialize the first state -- horrible side effect
    
    auto characterTile = gameplaySubsystem->GetTile(mSelectedCharacter); // could be different from currentTile
    characterTile->RemoveAllState();

    WatchCurrentTile();

    auto character = mSelectedCharacter;
    mSelectedCharacter = nullptr;
    turnSubsystem->ProcessFinishCharacter(character);
}


void ATurnBasedGamePlayerController::OnEndTurn(bool isPlayerTurn)
{
    // disable inputs
    DisableInput(this);
}


void ATurnBasedGamePlayerController::OnStartNewTurn(bool isPlayerTurn)
{
    if (isPlayerTurn)
    {
        auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
        auto gridManipulator = GetGridManipulator();

        if (!gameplaySubsystem || !gridManipulator)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnStartNewTurn - player turn - invalid subsystem"));
            return;
        }

        // get a character
        auto characters = gameplaySubsystem->GetPlayerCharacters();
        if (characters.Num() <= 0)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnStartNewTurn - no more player character"));
            return;
        }
        auto character = characters[0];
        if (!character)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnStartNewTurn - invalid character"));
            return;
        }

        //get tile
        auto tile = gameplaySubsystem->GetTile(character);
        if (!tile)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnStartNewTurn - invalid tile"));
            return;
        }

        mCurrentTile = tile;
        gridManipulator->SelectTile(mCurrentTile, nullptr);
        WatchCurrentTile();
        
        mStateStack.Empty();
        mStateStack.Add(GetDefaultState());
        // 
        // 
        // look at it
        EnableInput(this);
    }
    else
    {
        // Basic, default AI just use the defense ability

        auto turnSubsystem = GetWorld()->GetSubsystem<UTurnSubsystem>();
        auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();

        if (!turnSubsystem || !gameplaySubsystem)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnStartNewTurn - invalid subsystem"));
            return;
        }


        // get all enemy character
        auto enemyCharacters = gameplaySubsystem->GetEnemyCharacters();

        // get them to play defend
        for (auto enemy : enemyCharacters)
        {
            mSelectedCharacter = enemy;
            auto tile = gameplaySubsystem->GetTile(enemy);
            mCurrentTile->RemoveAllState();
            mCurrentTile = tile;
            WatchCurrentTile();

            AbilityHelper::TryActivateAbilitybyHiearchicalClass<UGameAbility_Defend>(enemy->GetAbilitySystemComponent());
        }
    }
}


UGridManipulatorSubsystem* ATurnBasedGamePlayerController::GetGridManipulator() const
{
    return GetWorld()->GetSubsystem<UGridManipulatorSubsystem>();
}
