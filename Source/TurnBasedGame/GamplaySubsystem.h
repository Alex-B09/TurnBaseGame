// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameCharacter.h"
#include "GridTile.h"
#include "GameGrid.h"

#include "GamplaySubsystem.generated.h"

UENUM()
enum class EGridTileState : uint8
{
    Empty,
    IsCharacterPlayer,
    IsCharacterEnemy,
};

UCLASS()
class UCharacterGridInfo : public UObject
{
    GENERATED_BODY()

public:
    // acharacter
    UPROPERTY()
        AGameCharacter* mCharacter;

    UPROPERTY()
        AGridTile* mTile;

    UPROPERTY()
        bool mIsPlayerCharacter;
};


/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UGamplaySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

private:
    UPROPERTY()
        TArray<UCharacterGridInfo*> mCharacters;

    UPROPERTY()
        AGameGrid* mGrid;

    //UPROPERTY()
    //	TArray<AGameCharacter> mPlayerCharacters;
    //UPROPERTY()
    //	TArray<AGameCharacter> mEnemyCharacters;

public:
    void SetupGrid(AGameGrid* grid);
    void AddCharacter(TSubclassOf<AGameCharacter> characterClass, AGridTile* tile, bool isPlayerControllable);


    EGridTileState GetTileStatus(AGridTile* tile) const;
    AGameCharacter* GetCharacter(AGridTile* tile) const;
    AGridTile* GetTile(AGameCharacter* character) const;

    UFUNCTION(BlueprintCallable)
        void HighlighGridForCharacter(AGameCharacter* character) const;

    UFUNCTION(BlueprintCallable)
        void HideGridHighlight() const;

    UFUNCTION(BlueprintCallable)
        void MoveCharacter(AGameCharacter* character, AGridTile* tileToMoveTo);
private:
    UCharacterGridInfo* GetGridInfo(AGridTile* tile) const;
    UCharacterGridInfo* GetGridInfo(AGameCharacter* character) const;
};
