// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridTile.h"
#include "GameCharacter.h"
#include "Helpers/GridPosition.h"

#include "GameGrid.generated.h"


// BP information for positioning characters
USTRUCT(BlueprintType)
struct FStartinPosition
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Starting Position"))
        FGridPosition mGridPosition;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Is Player Controlled"))
        bool mIsPlayerControlled; // true if the player can act with it
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Character Class"))
        TSubclassOf<AGameCharacter> mCharacter;
};

UENUM()
enum class ETileDirection : uint8
{
    Up,
    Left,
    Down,
    Right,
    None
};


UCLASS()
class TURNBASEDGAME_API AGameGrid : public AActor
{
    GENERATED_BODY()

private:
    // grid dimension
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic | dimension", meta = (AllowPrivateAccess = true, DisplayName = "NbSquareWidth"))
        int mNbWidth; // x
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic | dimension", meta = (AllowPrivateAccess = true, DisplayName = "NbSquareDepth"))
        int mNbDepth; // y


    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Tiles"))
        TArray<AGridTile*> mTiles;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayLogic", meta = (AllowPrivateAccess = true, DisplayName = "Tile Class"))
        TSubclassOf<AGridTile> mTileActorClass;

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

    

    TArray<AGridTile*> GetTiles(AGridTile* centerTile, int distanceFromTile);
    TArray<AGridTile*> GetTiles() const;

    std::pair<ETileDirection, int> GetTileDirection(AGridTile* source, AGridTile* target);



protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;


private:
    std::pair<int, int> GetXYPosition(int arrayPos); // easier to use pair then to create a new struct
};
