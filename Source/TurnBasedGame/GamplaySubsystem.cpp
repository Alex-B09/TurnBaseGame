// Fill out your copyright notice in the Description page of Project Settings.


#include "GamplaySubsystem.h"
#include <Runtime/AIModule/Classes/AIController.h>
#include <Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h>

void UGamplaySubsystem::SetupGrid(AGameGrid* grid)
{
    if (grid == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::SetupGrid - Invalid grid"));
        return;
    }

    mGrid = grid;
}



void UGamplaySubsystem::AddCharacter(TSubclassOf<AGameCharacter> characterClass, AGridTile* tile, bool isPlayerControllable)
{
    // reject if already occupied
    if (auto info = GetGridInfo(tile))
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::AddCharacter - tile already occupied -- entry rejected"));
        return;
    }

    // add function to get data

    auto transform = tile->GetCharacterPosition();
    auto characterActor = Cast<AGameCharacter>(GetWorld()->SpawnActor(characterClass, &transform, FActorSpawnParameters()));
    characterActor->SpawnDefaultController();
    auto info = NewObject<UCharacterGridInfo>();

    if (characterActor == nullptr || info == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::AddCharacter - invalid object -- need debug"));
    }

    info->mCharacter = characterActor;
    info->mTile = tile;
    info->mIsPlayerCharacter = isPlayerControllable;

    mCharacters.Add(info);
}

EGridTileState UGamplaySubsystem::GetTileStatus(AGridTile* tile) const
{
    if (!tile)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::GetTileStatus - invalid tile"));
        return EGridTileState::Empty; // TODO - add error?
    }

    // search the mCharacters for the time

    if (auto info = GetGridInfo(tile))
    {
        if (info->mIsPlayerCharacter)
        {
            return EGridTileState::IsCharacterPlayer;
        }
        return EGridTileState::IsCharacterEnemy;
    }

    return EGridTileState::Empty;
}

UCharacterGridInfo* UGamplaySubsystem::GetGridInfo(AGridTile* tile) const
{
    if (tile == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::GetGridInfo - invalid tile"));
        return nullptr;
    }

    for (auto character : mCharacters)
    {
        if (character->mTile == tile)
        {
            return character;
        }
    }

    return nullptr;
}

UCharacterGridInfo* UGamplaySubsystem::GetGridInfo(AGameCharacter* character) const
{
    if (character == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::GetGridInfo - invalid character"));
        return nullptr;
    }

    for (auto info : mCharacters)
    {
        if (info->mCharacter == character)
        {
            return info;
        }
    }

    return nullptr;
}

void UGamplaySubsystem::MoveCharacter(AGameCharacter* character, AGridTile* tileToMoveTo)
{
    UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::MoveCharacter - moving character"));

    // verify if null
    if (character == nullptr || tileToMoveTo == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::MoveCharacter - invalid input"));
        return;
    }


    // get info
    if (auto info = GetGridInfo(character))
    {
        TArray<AGridTile*> positions; // TODO - get the path (tile by tile) to the destination
        positions.Add(tileToMoveTo);

        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::MoveCharacter - hum...."));
        info->mCharacter->Move(info->mTile, positions);

        info->mTile = tileToMoveTo;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::MoveCharacter - character not found"));
    }
}

AGameCharacter* UGamplaySubsystem::GetCharacter(AGridTile* tile) const
{
    if (tile == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::GetCharacter - invalid tile"));
    }

    for (auto info : mCharacters)
    {
        if (info->mTile == tile)
        {
            return info->mCharacter;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::GetCharacter - not found character"));
    return nullptr;
}

AGridTile* UGamplaySubsystem::GetTile(AGameCharacter* character) const
{
    if (!character)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::GetTile - invalid input"));
        return nullptr;
    }

    for (auto info : mCharacters)
    {
        if (info->mCharacter == character)
        {
            return info->mTile;
        }
    }
    return nullptr;
}


void UGamplaySubsystem::HighlighGridForCharacter(AGameCharacter* character) const
{
    mGrid->LightForMovement(GetTile(character), 3);
}

void UGamplaySubsystem::HideGridHighlight() const
{
    mGrid->HideSelectors();
}
