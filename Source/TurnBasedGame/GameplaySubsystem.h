// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameCharacter.h"
#include "GridTile.h"
#include "GameGrid.h"
#include "Helpers/GridPosition.h"

#include "GameplaySubsystem.generated.h"

UENUM()
enum class ETileOccupationState : uint8
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
class TURNBASEDGAME_API UGameplaySubsystem : public UWorldSubsystem
{
    // Subsystem are awesome
    //  they got automatic casting in BP and C++
    //  you just need the "GetWorld" and you can get the subsystem
    //  
    //  The subsystems give you the ability to have a cleaner architecture by putting related function together

    GENERATED_BODY()

private:
    UPROPERTY()
        TArray<UCharacterGridInfo*> mCharacters;
    UPROPERTY()
        AGameGrid* mGrid;
    
    UPROPERTY()
    	TArray<AGameCharacter*> mPlayerCharacters;
    UPROPERTY()
    	TArray<AGameCharacter*> mEnemyCharacters;

public:
    void SetupGrid(AGameGrid* grid);
    void AddCharacter(TSubclassOf<AGameCharacter> characterClass, AGridTile* tile, bool isPlayerControllable);

    TArray<AGridTile*> GetAvailableMovementTiles(AGameCharacter* character);
    TArray<AGridTile*> GetAvailableAttackTiles(AGameCharacter* character);
    
    ETileOccupationState GetTileOccupationStatus(AGridTile* tile) const;

    AGridTile* GetTile(AGameCharacter* character) const;

    // TODO remove after tests
    AGameCharacter* GetEnemyCharacter();

    void InitTurnSubsystem();
    const TArray<AGameCharacter*> GetPlayerCharacters() const;
    const TArray<AGameCharacter*> GetEnemyCharacters() const;

public:
    UFUNCTION(BlueprintCallable)
        void HighlighGridForMovement(AGameCharacter* character) const;

    UFUNCTION(BlueprintCallable)
        void HighlighGridForAttack(AGameCharacter* character) const;

    UFUNCTION(BlueprintCallable)
        void HideGridHighlight() const;
    
    UFUNCTION(BlueprintCallable)
        void HideGridForAttack(AGameCharacter* character) const;

    UFUNCTION(BlueprintCallable)
        void MoveCharacter(AGameCharacter* character, AGridTile* tileToMoveTo);

    UFUNCTION(BlueprintCallable)
        void TeleportCharacter(AGameCharacter* character, AGridTile* tileToTeleportTo);

    UFUNCTION(BlueprintCallable)
        AGameCharacter* GetCharacter(AGridTile* tile) const;

private:
    UCharacterGridInfo* GetGridInfo(AGridTile* tile) const;
    UCharacterGridInfo* GetGridInfo(AGameCharacter* character) const;
};
