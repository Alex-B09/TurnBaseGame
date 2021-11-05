// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridTile.h"
#include "GameCharacter.h"
#include "Helpers/GridPosition.h"

#include "GameGrid.generated.h"



USTRUCT(BlueprintType)
struct FStartinPosition
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Starting Position"))
        FGridPosition mGridPosition;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IsPlayerControlled"))
        bool mIsPlayerControlled;
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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Tiles"))
        TArray<AGridTile*> mTiles;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "TileActorClass"))
        TSubclassOf<AGridTile> mTileActorClass;

    UPROPERTY()
        AGridTile* mSelectedTile;


    // Character and position -- this should be in the GameMode
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Characters"))
        TArray<FStartinPosition> mCharacters;

public:
    // Sets default values for this actor's properties
    AGameGrid();

    AGridTile* GetTile(FGridPosition position);
    FGridPosition GetTilePosition(AGridTile*);

    int GetWidth() const { return mNbWidth; }
    int GetDepth() const { return mNbDepth; }

    void LightForMovement(AGridTile* centerTile, int nbTile);
    void HideSelectors();

    TArray<AGridTile*> GetTiles(AGridTile* centerTile, int distanceFromTile);


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;


private:
    std::pair<int, int> GetXYPosition(int arrayPos);
};
