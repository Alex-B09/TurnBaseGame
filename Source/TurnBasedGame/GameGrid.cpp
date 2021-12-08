// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGrid.h"

#include "GameplaySubsystem.h"
#include "GridManipulatorSubsystem.h"

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
    auto gridSubsystem = GetWorld()->GetSubsystem<UGridManipulatorSubsystem>();
    if (!gameplaySubsystem || !gridSubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("AGameGrid::BeginPlay -- invalid subsystem"));
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

    gridSubsystem->SetGrid(this);
    gameplaySubsystem->InitTurnSubsystem();
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
    auto [x, y] = GetXYPosition(index); // need C++17 to work -- check TurnBasedGame.Build.cs

    return FGridPosition{ x,y };
}

std::pair<int, int> AGameGrid::GetXYPosition(int arrayPos)
{
    return std::pair<int, int>(arrayPos / mNbWidth, arrayPos % mNbWidth);
}

TArray<AGridTile*> AGameGrid::GetTiles(AGridTile* centerTile, int distanceFromTile)
{
    if (!centerTile || distanceFromTile <= 0)
    {
        UE_LOG(LogTemp, Log, TEXT("AGameGrid::GetTiles -- invalid input"));
        return {};
    }

    TArray<AGridTile*> tiles;
    int position = mTiles.Find(centerTile);

    // get x,y position
    auto [posX, posY] = GetXYPosition(position);

    for (int index = 0; index < mTiles.Num(); ++index)
    {
        // its easier to go that way then to do a find for each tile

        auto [x, y] = GetXYPosition(index);
        int distance = FMath::Abs(x - posX) + FMath::Abs(y - posY);
        if (distance <= distanceFromTile)
        {
            tiles.Add(mTiles[index]);
        }
    }

    return tiles;
}


TArray<AGridTile*> AGameGrid::GetTiles() const
{
    return mTiles;
}


std::pair<ETileDirection, int> AGameGrid::GetTileDirection(AGridTile* source, AGridTile* target)
{
    auto sourcePosition = GetTilePosition(source);
    auto targetPosition = GetTilePosition(target);

    ETileDirection direction = ETileDirection::None;

    // if it is up, y-source is lower then y-target
    if (sourcePosition.mPosY < targetPosition.mPosY)
    {
        direction = ETileDirection::Up;
    }
    
    if (sourcePosition.mPosY > targetPosition.mPosY)
    {
        direction = ETileDirection::Down;
    }

    // if it is right, x-source is lower then x-target
    if (sourcePosition.mPosX < targetPosition.mPosX)
    {
        direction = ETileDirection::Right;
    }

    if (sourcePosition.mPosX > targetPosition.mPosX)
    {
        direction = ETileDirection::Left;
    }

    int distance = FMath::Abs(sourcePosition.mPosX - targetPosition.mPosX) 
                 + FMath::Abs(sourcePosition.mPosY - targetPosition.mPosY);


    return std::pair<ETileDirection, int>(direction, distance);
}
