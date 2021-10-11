// Fill out your copyright notice in the Description page of Project Settings.


#include "GamplaySubsystem.h"

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
