// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedGamePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
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

void ATurnBasedGamePlayerController::OnMoveDown()
{
	UE_LOG(LogTemp, Log, TEXT("Move down"));

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

void ATurnBasedGamePlayerController::OnMoveLeft()
{
	UE_LOG(LogTemp, Log, TEXT("Move left"));

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

void ATurnBasedGamePlayerController::OnMoveRight()
{
	UE_LOG(LogTemp, Log, TEXT("Move Right"));

	int depth = mGrid->GetWidth();

	if (mCurrentX +1 < depth)
	{
		mCurrentX += 1;
		WatchCurrentTile();
	}
	else
	{
		OnMovementError();
	}
}

void ATurnBasedGamePlayerController::OnAction()
{
	UE_LOG(LogTemp, Log, TEXT("Action"));


	if (mState == EControllerActionState::Selecting)
	{
		auto gameplaySubsystem = GetWorld()->GetSubsystem<UGamplaySubsystem>();

		auto tileStatus = gameplaySubsystem->GetTileStatus(GetCurrentTile());

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
			UE_LOG(LogTemp, Log, TEXT("Selected"));
		}
	}
}

void ATurnBasedGamePlayerController::OnCancel()
{
	UE_LOG(LogTemp, Log, TEXT("Cancel"));

	if (mState == EControllerActionState::Selected)
	{
		UE_LOG(LogTemp, Log, TEXT("Reverting to selecting"));
		mState = EControllerActionState::Selecting;
	}
}

void ATurnBasedGamePlayerController::WatchCurrentTile()
{
	// TODO get old tile too....

	if (mGrid)
	{
		if (auto tile = GetCurrentTile())
		{
			UE_LOG(LogTemp, Log, TEXT("ATurnBasedGamePlayerController::WatchCurrentTile - watch x:%d y:%d"), mCurrentX, mCurrentY);

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

//void ATurnBasedGamePlayerController::SetNewMoveDestination(const FVector DestLocation)
//{
//	APawn* const MyPawn = GetPawn();
//	if (MyPawn)
//	{
//		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());
//
//		// We need to issue move command only if far enough in order for walk animation to play correctly
//		if ((Distance > 50.0f))
//		{
//			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
//		}
//	}
//}
