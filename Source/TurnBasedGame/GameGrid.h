// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridTile.h"
#include "GameCharacter.h"
#include "GameGrid.generated.h"


USTRUCT(BlueprintType)
struct FStartinPosition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "X"))
		int mPosX; // x
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Y"))
		int mPosY; // y
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IsPlayerControlled"))
		bool mIsPlayerControlled; // y
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GameCharacters"))
		TSubclassOf<AGameCharacter> mCharacter;
};


UCLASS()
class TURNBASEDGAME_API AGameGrid : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "NbSquareWidth"))
		int mNbWidth; // x
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "NbSquareDepth"))
		int mNbDepth; // y

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Squares"))
		TArray<AGridTile*> mTiles;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "SquareActorClass"))
		TSubclassOf<class AGridTile> mSquareActorClass;
    
	UPROPERTY()
		AGridTile* mSelectedTile;


	// Character and position
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Characters"))
		TArray<FStartinPosition> mCharacters;

public:	
	// Sets default values for this actor's properties
	AGameGrid();

	AGridTile* GetTile(int x, int y);

	bool SelectTile(AGridTile* tile);
	bool SelectTile(int x, int y);


	int GetWidth() const { return mNbWidth; }
	int GetDepth() const { return mNbDepth; }

	void LightForMovement(AGridTile* tile, int nbTile);
	void HideSelectors();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	std::pair<int, int> GetXYPosition(int arrayPos);
};
