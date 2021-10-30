// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedGamePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include <string>
#include <TurnBasedGame/GamplaySubsystem.h>


ATurnBasedGamePlayerController::ATurnBasedGamePlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATurnBasedGamePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // get the grid
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameGrid::StaticClass(), foundActors);

    if (foundActors.Num() > 0)
    {
        if (auto grid = Cast<AGameGrid>(foundActors[0]))
        {
            mGrid = grid;
            WatchCurrentTile();
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::BeginPlay - invalid grid"));
        }
    }
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

    if (mState == EControllerActionState::Selected)
    {
        //OnMoveUp_UI();
    }
    else
    {
        int width = mGrid->GetDepth();

        if (mCurrentY + 1 < width)
        {
            mCurrentY += 1;
            WatchCurrentTile();
        }
        else
        {
            OnMovementError();
        }
    }
}

void ATurnBasedGamePlayerController::OnMoveDown()
{
    UE_LOG(LogTemp, Log, TEXT("Move down"));

    if (mState == EControllerActionState::Selected)
    {
        //OnMoveDown_UI();
    }
    else
    {
        if (mCurrentY - 1 >= 0)
        {
            mCurrentY -= 1;
            WatchCurrentTile();
        }
        else
        {
            OnMovementError();
        }
    }
}

void ATurnBasedGamePlayerController::OnMoveLeft()
{
    UE_LOG(LogTemp, Log, TEXT("Move left"));

    if (mState == EControllerActionState::Selected)
    {
        //OnMoveLeft_UI();
    }
    else
    {
        if (mCurrentX - 1 >= 0)
        {
            mCurrentX -= 1;
            WatchCurrentTile();
        }
        else
        {
            OnMovementError();
        }
    }
}

void ATurnBasedGamePlayerController::OnMoveRight()
{
    UE_LOG(LogTemp, Log, TEXT("Move Right"));

    if (mState == EControllerActionState::Selected)
    {
        //OnMoveRight_UI();
    }
    else
    {
        int depth = mGrid->GetWidth();

        if (mCurrentX + 1 < depth)
        {
            mCurrentX += 1;
            WatchCurrentTile();
        }
        else
        {
            OnMovementError();
        }
    }
}

void ATurnBasedGamePlayerController::OnAction()
{
    UE_LOG(LogTemp, Log, TEXT("Action"));

    if (mState == EControllerActionState::Selected)
    {
        //OnAction_UI();
    }
    else
    {
        auto gameplaySubsystem = GetWorld()->GetSubsystem<UGamplaySubsystem>();

        auto tile = GetCurrentTile();
        if (mState == EControllerActionState::Selecting)
        {
            auto tileStatus = gameplaySubsystem->GetTileStatus(tile);

            if (tileStatus == EGridTileState::Empty)
            {
                UE_LOG(LogTemp, Log, TEXT("Tile Empty"));
            }
            else if (tileStatus == EGridTileState::IsCharacterEnemy)
            {
                UE_LOG(LogTemp, Log, TEXT("Tile enemy"));
            }
            else if (tileStatus == EGridTileState::IsCharacterPlayer)
            {
                UE_LOG(LogTemp, Log, TEXT("Tile character"));
    
                // in mode selected
                mState = EControllerActionState::Selected;
    
                if (auto character = gameplaySubsystem->GetCharacter(tile))
                {
                    mSelectedCharacter = character;
                    tile->SetToCharacterSelected();
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::OnAction - invalid character returned"));
                }
                ShowActionMenu();
    
                UE_LOG(LogTemp, Log, TEXT("Selected"));
            }
        }
        else if (mState == EControllerActionState::Moving)
        {
            OnTileSelect.Broadcast(tile);

            //auto tileStatus = gameplaySubsystem->GetTileStatus(tile);

            //// if unocupied tile
            //if (tileStatus == EGridTileState::Empty)
            //{
            //	// move character
            //	mGrid->HideSelectors();
            //	gameplaySubsystem->MoveCharacter(mSelectedCharacter, tile);
            //	tile->SetToCharacterSelected();
            //}
        }
    }
}

void ATurnBasedGamePlayerController::OnCancel()
{
    UE_LOG(LogTemp, Log, TEXT("Cancel"));

    if (mState == EControllerActionState::Selected)
    {
        UE_LOG(LogTemp, Log, TEXT("Reverting to selecting"));
        
        // revert to selecting
        //OnCancel_UI();
    }
    else
    {
        if (mState == EControllerActionState::Moving)
        {
            OnCancelled.Broadcast();
            mState = EControllerActionState::Selecting;
        }
        // hide movement
        mGrid->HideSelectors();
    }
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
				UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::WatchCurrentTile - x:%d y:%d -- error on select tile -- should never happen"), mCurrentX, mCurrentY);
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
	return mGrid->GetTile(mCurrentX, mCurrentY);
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

void ATurnBasedGamePlayerController::SetupNewState()
{
    // create the right state
    // assume it was created?
    if (!mControllerState)
    {
        UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::SetupNewState - invalid state"));
        return;
    }

    // AddDynamic does not work...maybe it's something releated to events?
    //  meh...lambda works...
    mControllerState->OnTileChanged().AddLambda([this]()
                                                {
                                                    OnStateTileChanged();
                                                });



}

void ATurnBasedGamePlayerController::OnStateTileChanged()
{

}
