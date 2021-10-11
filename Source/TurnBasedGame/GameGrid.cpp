// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGrid.h"
#include <TurnBasedGame/GamplaySubsystem.h>

// Sets default values
AGameGrid::AGameGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGameGrid::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UChildActorComponent*> components;
	GetComponents<UChildActorComponent>(components);

	for (auto component : components)
	{
		// if the component is an instance of the selected class
		if (component->GetChildActor()->IsA(mSquareActorClass->GetDefaultObject()->StaticClass()))
		{
			mTiles.Add(Cast<AGridTile>(component->GetChildActor()));
		}
	}

	auto gameplaySubsystem = GetWorld()->GetSubsystem<UGamplaySubsystem>();
	if (gameplaySubsystem == nullptr)
	{
		UE_LOG(LogTemp,Log,TEXT("AGameGrid::BeginPlay -- invalid gameplaySubsystem"));
		return;
	}

	for (auto &character : mCharacters)
	{
		if (auto tile = GetTile(character.mPosX, character.mPosY))
		{
			gameplaySubsystem->AddCharacter(character.mCharacter, tile, character.mIsPlayerControlled);
		}
		else
		{
            UE_LOG(LogTemp, Log, TEXT("AGameGrid::BeginPlay -- error on character placement"));
		}
	}


}


AGridTile* AGameGrid::GetTile(int x, int y)
{
	if (x >= 0 && x < mNbWidth
		&& y >= 0 && y < mNbDepth)
	{
		return mTiles[x * mNbDepth + y];
	}
	return nullptr;
}

bool AGameGrid::SelectTile(AGridTile* tile)
{
	if (tile)
	{
		tile->Selected(true);

		if (mSelectedTile)
		{
			mSelectedTile->Selected(false);
		}
		mSelectedTile = tile;
		return true;
	}
	return false;
}

bool AGameGrid::SelectTile(int x, int y)
{
	return SelectTile(GetTile(x, y));
}

