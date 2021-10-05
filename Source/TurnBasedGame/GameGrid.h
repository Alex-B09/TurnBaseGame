// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridTile.h"

#include "GameGrid.generated.h"

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
		TSoftClassPtr<class AGridTile> mSquareActorClass;


public:	
	// Sets default values for this actor's properties
	AGameGrid();

	//UFUNCTION(BlueprintCallable, BlueprintGetter, BlueprintPure, Category = "GameplayLogic")
		AGridTile* GetTile(int x, int y);


	int GetWidth() const { return mNbWidth; }
	int GetDepth() const { return mNbDepth; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
