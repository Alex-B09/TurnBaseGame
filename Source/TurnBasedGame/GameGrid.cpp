// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGrid.h"
#include <TurnBasedGame/GameplaySubsystem.h>

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

    GetWorld()->GetSubsystem<UGameplaySubsystem>()->SetupGrid(this);


    TArray<UChildActorComponent*> components;
    GetComponents<UChildActorComponent>(components);

    for (auto component : components)
    {
        // if the component is an instance of the selected class
        if (component->GetChildActor()->IsA(mTileActorClass->GetDefaultObject()->StaticClass()))
        {
            mTiles.Add(Cast<AGridTile>(component->GetChildActor()));
        }
    }

    auto gameplaySubsystem = GetWorld()->GetSubsystem<UGameplaySubsystem>();
    if (gameplaySubsystem == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("AGameGrid::BeginPlay -- invalid gameplaySubsystem"));
        return;
    }

    for (auto& character : mCharacters)
    {
        if (auto tile = GetTile(character.mGridPosition))
        {
            gameplaySubsystem->AddCharacter(character.mCharacter, tile, character.mIsPlayerControlled);
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("AGameGrid::BeginPlay -- error on character placement"));
        }
    }
}


AGridTile* AGameGrid::GetTile(FGridPosition position)
{
    int x = position.mPosX;
    int y = position.mPosY;

    if (x >= 0 && x < mNbWidth
        && y >= 0 && y < mNbDepth)
    {
        return mTiles[x * mNbDepth + y];
    }
    return nullptr;
}

FGridPosition AGameGrid::GetTilePosition(AGridTile* tile)
{
    // get index first
    int index = mTiles.Find(tile);
    auto [x, y] = GetXYPosition(index);

    return FGridPosition{ x,y };
}

void AGameGrid::LightForMovement(AGridTile* centerTile, int nbTile)
{
    // get tile position
    auto tiles = GetTiles(centerTile, nbTile);

    for (auto tile : tiles)
    {
        tile->SetToMovement();
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

TArray<AGridTile*> AGameGrid::GetTiles(AGridTile* centerTile, int distanceFromTile)
{
    TArray<AGridTile*> tiles;
    int position = mTiles.Find(centerTile);

    // get x,y position
    auto [posX, posY] = GetXYPosition(position);

    for (int index = 0; index < mTiles.Num(); ++index)
    {
        auto [x, y] = GetXYPosition(index);
        int distance = FMath::Abs(x - posX) + FMath::Abs(y - posY);
        if (distance <= distanceFromTile)
        {
            tiles.Add(mTiles[index]);
        }
    }

    return tiles;
}
