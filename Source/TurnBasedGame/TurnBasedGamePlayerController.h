// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GameGrid.h"

#include "TurnBasedGamePlayerController.generated.h"

UCLASS()
class ATurnBasedGamePlayerController : public APlayerController
{
	GENERATED_BODY()


private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Grid"))
		AGameGrid* mGrid;

	int mCurrentX = 0;
	int mCurrentY = 0;

public:
	ATurnBasedGamePlayerController();


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

protected:
    UFUNCTION(BlueprintImplementableEvent)
		void OnWatchTile(AGridTile* toWatch);

	UFUNCTION(BlueprintImplementableEvent)
		void OnMovementError();

private:
	void WatchCurrentTile();

};
