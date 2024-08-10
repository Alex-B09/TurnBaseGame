// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplaySubsystem.h"

#include "TurnSubsystem.h"
#include "GridManipulatorSubsystem.h"

#include <Runtime/AIModule/Classes/AIController.h>
#include <Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h>

void UGameplaySubsystem::SetupGrid(AGameGrid* grid)
{
    if (grid == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGameplaySubsystem::SetupGrid - Invalid grid"));
        return;
    }

    mGrid = grid;
}


void UGameplaySubsystem::AddCharacter(TSubclassOf<AGameCharacter> characterClass, AGridTile* tile, bool isPlayerControllable)
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
    characterActor->SpawnDefaultController(); // TODO - test if i still need this
    auto info = NewObject<UCharacterGridInfo>();

    if (characterActor == nullptr || info == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::AddCharacter - invalid object -- need debug"));
        return;
    }

    info->mCharacter = characterActor;
    info->mTile = tile;
    info->mIsPlayerCharacter = isPlayerControllable;

    // this seems a bit overkill...but it works fine
    if (isPlayerControllable)
    {
        mPlayerCharacters.Add(characterActor);
    }
    else
    {
        mEnemyCharacters.Add(characterActor);
    }

    mCharacters.Add(info);
}


ETileOccupationState UGameplaySubsystem::GetTileOccupationStatus(AGridTile* tile) const
{
    if (!tile)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::GetTileStatus - invalid tile"));
        return ETileOccupationState::Empty; // TODO - add error?
    }

    if (auto info = GetGridInfo(tile))
    {
        if (info->mIsPlayerCharacter)
        {
            return ETileOccupationState::IsCharacterPlayer;
        }
        return ETileOccupationState::IsCharacterEnemy;
    }

    return ETileOccupationState::Empty;
}


UCharacterGridInfo* UGameplaySubsystem::GetGridInfo(AGridTile* tile) const
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


UCharacterGridInfo* UGameplaySubsystem::GetGridInfo(AGameCharacter* character) const
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


void UGameplaySubsystem::MoveCharacter(AGameCharacter* character, AGridTile* tileToMoveTo)
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
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::MoveCharacter - test"));
        info->mTile = tileToMoveTo;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::MoveCharacter - character not found"));
    }
}


void UGameplaySubsystem::TeleportCharacter(AGameCharacter* character, AGridTile* tileToTeleportTo)
{
    UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::TeleportCharacter - teleporting character"));

    // verify if null
    if (character == nullptr || tileToTeleportTo == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::TeleportCharacter - invalid input"));
        return;
    }

    // get info
    if (auto info = GetGridInfo(character))
    {
        if (info->mTile != tileToTeleportTo)
        {
            info->mCharacter->Teleport(tileToTeleportTo);
            info->mTile = tileToTeleportTo;
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::TeleportCharacter - character not found"));
    }
}


AGameCharacter* UGameplaySubsystem::GetCharacter(AGridTile* tile) const
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

    return nullptr;
}


AGridTile* UGameplaySubsystem::GetTile(AGameCharacter* character) const
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


TArray<AGridTile*> UGameplaySubsystem::GetAvailableMovementTiles(AGameCharacter* character)
{
    if (!character)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::GetAvailableMovementTiles - invalid character"));
        return {};
    }

    auto tile = GetTile(character);
    
    // TODO - get speed (or distance or whatnot) from attributes
    
    auto availableTiles = mGrid->GetTiles(tile, 4); // the 4 here is temporary 
    return availableTiles;
}


// TODO this needs to change to be provided by the weapon -- when the weapon logic will be implemented
TArray<FAttackTarget> UGameplaySubsystem::GetAvailableAttackTarget(AGameCharacter* character)
{
    auto tile = GetTile(character);
    auto availableTiles = mGrid->GetTiles(tile, 1); // replace the 1 to use the abilitySet or weapon -- if ever used
    
    // remove current tile from list
    availableTiles.Remove(tile);
    

    TArray<FAttackTarget> targets;

    for (auto attackTile : availableTiles)
    {
        FAttackTarget target;
        target.mCharacter = GetCharacter(attackTile);
        target.mTile = attackTile;

        auto [direction, distance] = mGrid->GetTileDirection(tile, attackTile);
        target.mDirection = direction;
        target.mDistance = distance;

        targets.Add(target);
    }

    // reorganise by:
    //      up, right, down, left

    targets.Sort([=](const FAttackTarget& left, const FAttackTarget& right)
                 {
                     bool isLeftFirst = left.mDirection < right.mDirection;
                     bool isRightFirst = left.mDirection > right.mDirection;
                     bool isLeftCloser = left.mDistance < right.mDistance;

                     if (isLeftFirst)
                     {
                         return true;
                     }
                     if (isRightFirst)
                     {
                         return false;
                     }

                     return isLeftCloser;
                 });

    return targets;
}


AGameCharacter* UGameplaySubsystem::GetEnemyCharacter()
{
    for (auto character : mCharacters)
    {
        if (!character->mIsPlayerCharacter)
        {
            return character->mCharacter;
        }
    }
    return nullptr;
}


void UGameplaySubsystem::InitTurnSubsystem()
{
    // maybe remove this from here and juste let the turn subsystem initialize itself?
    auto turnSubsystem = GetWorld()->GetSubsystem<UTurnSubsystem>();
    if (!turnSubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("UGamplaySubsystem::InitTurnSubsystem - invalid turn subsystem"));
        return;
    }

    turnSubsystem->GoToNextTeamTurn();
}


const TArray<AGameCharacter*> UGameplaySubsystem::GetPlayerCharacters() const
{
    return mPlayerCharacters;
}


const TArray<AGameCharacter*> UGameplaySubsystem::GetEnemyCharacters() const
{
    return mEnemyCharacters;
}
