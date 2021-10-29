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

    GetWorld()->GetSubsystem<UGamplaySubsystem>()->SetupGrid(this);

	
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
		tile->SetToSelection();

		if (mSelectedTile)
		{
			mSelectedTile->RemoveLastState();
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

void AGameGrid::LightForMovement(AGridTile* tile, int nbTile)
{
	// get tile position

    auto asdf = mTiles.Num();

	int position = mTiles.Find(tile);

	// get x,y position
	auto [posX, posY] = GetXYPosition(position);
	
	for (int index = 0; index < mTiles.Num(); ++index)
	{
		auto [x, y] = GetXYPosition(index);
		int distance = FMath::Abs(x - posX) + FMath::Abs(y - posY);
		if (distance <= nbTile)
		{
			mTiles[index]->SetToMovement();
		}
	}
}

void AGameGrid::HideSelectors()
{
	for (auto tile : mTiles)
	{
		tile->RemoveLastState();
	}
}

std::pair<int, int> AGameGrid::GetXYPosition(int arrayPos)
{
	return std::pair<int, int>(arrayPos / mNbWidth, arrayPos % mNbWidth);
}