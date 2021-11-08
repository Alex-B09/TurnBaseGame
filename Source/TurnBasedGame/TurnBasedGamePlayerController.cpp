// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedGamePlayerController.h"

// states
#include "ControllerStates/ControllerState_Selecting.h"
#include "ControllerStates/ControllerState_UI.h"
#include "ControllerStates/ControllerState_Movement.h"

#include "Abilities/GameAbility_Movement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Helpers/TagsConst.h"
#include "Helpers/AbilityHelper.h"


#include <string>
#include "GameplaySubsystem.h"


ATurnBasedGamePlayerController::ATurnBasedGamePlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
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

    mCurrentTile = mGrid->GetTile(FGridPosition{ 0,0 });

    if (!mCurrentTile)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::BeginPlay - invalid startup tile"));
        return;
    }

    SetupFirstState();
    WatchCurrentTile();
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

    if (!mControllerState)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnMoveUp - state error"));
        return;
    }

    mControllerState->OnMoveUp();
}

void ATurnBasedGamePlayerController::OnMoveDown()
{
    UE_LOG(LogTemp, Log, TEXT("Move down"));

    if (!mControllerState)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnMoveDown - state error"));
        return;
    }

    mControllerState->OnMoveDown();
}

void ATurnBasedGamePlayerController::OnMoveLeft()
{
    UE_LOG(LogTemp, Log, TEXT("Move left"));

    if (!mControllerState)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnMoveLeft - state error"));
        return;
    }

    mControllerState->OnMoveLeft();
}

void ATurnBasedGamePlayerController::OnMoveRight()
{
    UE_LOG(LogTemp, Log, TEXT("Move Right"));

    if (!mControllerState)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnMoveRight - state error"));
        return;
    }

    mControllerState->OnMoveRight();
}

void ATurnBasedGamePlayerController::OnAction()
{
    UE_LOG(LogTemp, Log, TEXT("Action"));

    if (!mControllerState)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnAction - state error"));
        return;
    }

    mControllerState->OnAction();
}

void ATurnBasedGamePlayerController::OnCancel()
{
    UE_LOG(LogTemp, Log, TEXT("Cancel"));

    if (!mControllerState)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnCancel - state error"));
        return;
    }

    mControllerState->OnCancel();
}

void ATurnBasedGamePlayerController::WatchCurrentTile()
{
	if (mGrid)
	{
		if (auto tile = GetCurrentTile())
		{
            tile->SetToSelection();
            OnWatchTile(tile);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::WatchCurrentTile - invalid tile"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::WatchCurrentTile - invalid grid"));
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


void ATurnBasedGamePlayerController::SetMovementMode()
{
    if (!mSelectedCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SetMovementMode - invalid input"));
        return;
    }

    // activate ability
    auto abilities = mSelectedCharacter->GetAbilitySystemComponent()->GetActivatableAbilities();
    AbilityHelper::TryActivateAbilitybyHiearchicalClass<UGameAbility_Movement>(mSelectedCharacter->GetAbilitySystemComponent());

    auto state = NewObject<UControllerState_Movement>();

    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    auto availableTiles = gameplaySubsystem->GetAvailableMovementTiles(mSelectedCharacter);
    auto tile = GetCurrentTile();

    state->Setup(mCurrentTile, mGrid, availableTiles);

    auto changeToMenuStateLambda = [=]()
    {
        // go to ui mode  
        ShowActionMenu();

        if (!mWidget)
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnCharacterSelected - invalid widget"));
            return;
        }

        auto state = NewObject<UControllerState_UI>();
        state->Setup(mWidget);

        mControllerState = state;
    };

    auto emptyTileEventLambda = [=]()
                                {
                                    OnTileSelect.Broadcast(GetCurrentTile());
                                    // register to movement finished

                                    auto emptyEventsLambda = [=]
                                                             {
                                                                 mSelectedCharacter->OnStartedMovement().Clear();
                                                                 mSelectedCharacter->OnFinishMovement().Clear();
                                                                 changeToMenuStateLambda();
                                                             };

                                    mSelectedCharacter->OnFinishMovement().AddLambda(changeToMenuStateLambda);
                                    mSelectedCharacter->OnFinishMovement().AddLambda(emptyEventsLambda);
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

        changeToMenuStateLambda();
    };

    auto cancelEventLambda = [=]()
                             {
                                 OnCancelled.Broadcast();

                                 mCurrentTile = tile;
                                 tile->SetToCharacterSelected();
                                 WatchCurrentTile();

                                 SetupFirstState();
                             };

    state->OnEmptyTileSelected().AddLambda(emptyTileEventLambda);
    state->OnCancelSelected().AddLambda(cancelEventLambda);
    state->OnCharacterSelected().AddLambda(selfSelectLambda);

    state->OnTileChanged().AddLambda([=](AGridTile* newTile)
                                     {
                                         SwitchCurrentTile(newTile);
                                     });

    mControllerState = state;
}

void ATurnBasedGamePlayerController::SetupFirstState()
{
    UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SetupNewState - setup basic selection state"));
    
    auto state = NewObject<UControllerState_Selecting>();
    state->Setup(mCurrentTile, mGrid);

    // AddDynamic does not work...maybe it's something releated to events?
    //  meh...lambda works...
    state->OnTileChanged().AddLambda([=](AGridTile* newTile)
                                     {
                                         SwitchCurrentTile(newTile);
                                     });

    state->OnCharacterSelected().AddLambda([=]()
                                           {
                                               OnCharacterSelected();
                                           });

    mControllerState = state;
}

void ATurnBasedGamePlayerController::OnCharacterSelected()
{
    // change mode to ui
    // setup new state
    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    auto tile = GetCurrentTile();

    if (auto character = gameplaySubsystem->GetCharacter(tile))
    {
        mSelectedCharacter = character;
        tile->SetToCharacterSelected();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnAction - invalid character returned"));
        return;
    }



    SetMovementMode();

    //// setup new state
    
}

void ATurnBasedGamePlayerController::ProcessUIAction(FGameplayTag tag)
{
    if (tag.GetTagName() == TagConst::UI_MOVEMENT)
    {
        
    }
}

void ATurnBasedGamePlayerController::SwitchCurrentTile(AGridTile* newTile)
{
    mCurrentTile->RemoveLastState();
    mCurrentTile = newTile;
    WatchCurrentTile();
}
