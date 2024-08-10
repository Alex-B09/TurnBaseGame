// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GameGrid.h"

#include "Subsystems/WorldSubsystem.h"



#include "GridManipulatorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UGridManipulatorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
private:
    UPROPERTY()
        AGameGrid* mGrid;
public:
    void Initialize(FSubsystemCollectionBase& Collection) override;
    void SetGrid(AGameGrid* grid);

    UFUNCTION(BlueprintCallable)
        void HighlighTilesForMovement(TArray<AGridTile*> movementTiles) const;
    UFUNCTION(BlueprintCallable)
        void SelectForMovement(AGridTile* selectedMovementTile, AGridTile* oldMovementTile) const;
    

    UFUNCTION(BlueprintCallable)
        void HighlighTilesForAttack(TArray<AGridTile*> attackTiles) const;
    UFUNCTION(BlueprintCallable)
        void SelectTileForAttack(AGridTile* selectedAttackTile, AGridTile* oldAttackTile) const;
    UFUNCTION(BlueprintCallable)
        void RevertAttackState();


    UFUNCTION(BlueprintCallable)
        void SelectTile(AGridTile* selectedTile, AGridTile* oldSelectedTile) const;
    UFUNCTION(BlueprintCallable)
        void RemoveTilesOverlays();
    
    UFUNCTION(BlueprintCallable)
        void HighlightCharacter(AGridTile* selectedTile);
    UFUNCTION(BlueprintCallable)
        void RemoveCharacterHighlight(AGridTile* selectedTile);
};
