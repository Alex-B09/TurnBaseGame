// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedGamePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// states
#include "ControllerStates/ControllerState_Selecting.h"

#include <string>
#include <TurnBasedGame/GameplaySubsystem.h>


ATurnBasedGamePlayerController::ATurnBasedGamePlayerController()
    :mPosition{0,0}
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
        }
    }

    SetupFristState();
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

    //if (mState == EControllerActionState::Selected)
    //{
    //    //OnAction_UI();
    //}
    //else
    //{
    //    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();

    //    auto tile = GetCurrentTile();
    //    if (mState == EControllerActionState::Selecting)
    //    {
    //        auto tileStatus = gameplaySubsystem->GetTileStatus(tile);

    //        if (tileStatus == EGridTileState::Empty)
    //        {
    //            UE_LOG(LogTemp, Log, TEXT("Tile Empty"));
    //        }
    //        else if (tileStatus == EGridTileState::IsCharacterEnemy)
    //        {
    //            UE_LOG(LogTemp, Log, TEXT("Tile enemy"));
    //        }
    //        else if (tileStatus == EGridTileState::IsCharacterPlayer)
    //        {
    //            UE_LOG(LogTemp, Log, TEXT("Tile character"));
    //
    //            // in mode selected
    //            mState = EControllerActionState::Selected;
    //
    //            if (auto character = gameplaySubsystem->GetCharacter(tile))
    //            {
    //                mSelectedCharacter = character;
    //                tile->SetToCharacterSelected();
    //            }
    //            else
    //            {
    //                UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnAction - invalid character returned"));
    //            }
    //            ShowActionMenu();
    //
    //            UE_LOG(LogTemp, Log, TEXT("Selected"));
    //        }
    //    }
    //    else if (mState == EControllerActionState::Moving)
    //    {
    //        OnTileSelect.Broadcast(tile);

    //        //auto tileStatus = gameplaySubsystem->GetTileStatus(tile);

    //        //// if unocupied tile
    //        //if (tileStatus == EGridTileState::Empty)
    //        //{
    //        //	// move character
    //        //	mGrid->HideSelectors();
    //        //	gameplaySubsystem->MoveCharacter(mSelectedCharacter, tile);
    //        //	tile->SetToCharacterSelected();
    //        //}
    //    }
    //}
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

    //if (mState == EControllerActionState::Selected)
    //{
    //    UE_LOG(LogTemp, Log, TEXT("Reverting to selecting"));
    //    
    //    // revert to selecting
    //    //OnCancel_UI();
    //}
    //else
    //{
    //    if (mState == EControllerActionState::Moving)
    //    {
    //        OnCancelled.Broadcast();
    //        mState = EControllerActionState::Selecting;
    //    }
    //    // hide movement
    //    mGrid->HideSelectors();
    //}
}

void ATurnBasedGamePlayerController::WatchCurrentTile()
{
	if (mGrid)
	{
		if (auto tile = GetCurrentTile())
		{
			//UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::WatchCurrentTile - watch x:%d y:%d"), mCurrentX, mCurrentY);

			if (mGrid->SelectTile(tile)) // should never be false...but...just to be sure
			{
				OnWatchTile(tile);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::WatchCurrentTile - x:%d y:%d -- error on select tile -- should never happen"), mPosition.mPosX, mPosition.mPosY);
			}
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
	return mGrid->GetTile(mPosition.mPosX, mPosition.mPosY);
}

void ATurnBasedGamePlayerController::SetMovementMode()
{
	mState = EControllerActionState::Moving;
}

AGameCharacter* ATurnBasedGamePlayerController::GetCharacter() const
{
    return mSelectedCharacter;
}

void ATurnBasedGamePlayerController::SetUIWidget(UInputWidget* widget)
{
    mWidget = widget;
}

void ATurnBasedGamePlayerController::SetupFristState()
{
    UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SetupNewState - invalid state"));
    
    auto state = NewObject<UControllerState_Selecting>();
    state->Setup(&mPosition, mGrid);

    state->OnTileChanged().AddLambda([=]()
                                        {
                                            WatchCurrentTile();
                                        });

    state->OnCharacterSelected().AddLambda([=]()
                                           {
                                               OnCharacterSelected();
                                           });
    

    mControllerState = state;
        

    //// AddDynamic does not work...maybe it's something releated to events?
    ////  meh...lambda works...
    //mControllerState->OnTileChanged().AddLambda([=]()
    //                                            {
    //                                                OnStateTileChanged();
    //                                            });
}

void ATurnBasedGamePlayerController::OnStateTileChanged()
{
    // do something here corresponding the the tile change
    // should it be done here though?
    // the states knows best
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

    //set new state


}
