// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GameGrid.h"
#include "TurnBasedGamePlayerController.generated.h"

UENUM()
enum class EControllerActionState : uint8
{
	Selecting,
	Selected,
	Moving,
	Attacking
};

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
	//	i'm not taking any chances
	UPROPERTY()
		int mCurrentX = 0;
	UPROPERTY()
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
	void OnCancel();

protected:
    UFUNCTION(BlueprintImplementableEvent)
		void OnWatchTile(AGridTile* toWatch);

	UFUNCTION(BlueprintImplementableEvent)
		void OnMovementError();

private:
	void WatchCurrentTile();
	AGridTile* GetCurrentTile() const;

};
