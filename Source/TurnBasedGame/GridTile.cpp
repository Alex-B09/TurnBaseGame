// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTile.h"


// Sets default values
AGridTile::AGridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mScenePosition = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = mScenePosition;

	mCharacterPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Character Position"));
	mCharacterPosition->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGridTile::BeginPlay()
{
	Super::BeginPlay();
}

void AGridTile::SetToSelection()
{
	if (mStates.Num() > 0)
	{
		if (mStates.Last() == ETileState::SelectedCharacter)
		{
			return; // dont do anything if it is the character tile
		}
	}
	mStates.Add(ETileState::Selected);
	BP_UpdateState();
}

void AGridTile::SetToMovement()
{
	mStates.Add(ETileState::SelectedForMovement);
	BP_UpdateState();
}

void AGridTile::SetToSpell()
{
	mStates.Add(ETileState::SelectedForMagic);
	BP_UpdateState();
}

void AGridTile::SetToCharacterSelected()
{
	// this is special
	RemoveAllState();
	mStates.Add(ETileState::SelectedCharacter);
	BP_UpdateState();
}

void AGridTile::SetToAttack()
{
	mStates.Add(ETileState::SelectedForAttack);
	BP_UpdateState();
}

void AGridTile::RemoveLastState()
{
	if (mStates.Num() > 0)
	{
		mStates.Pop(); // dont care about the result
	}

	BP_UpdateState();
}

void AGridTile::RemoveAllState()
{
	mStates.Empty();
	BP_UpdateState();
}

ETileState AGridTile::GetState() const
{
	if (mStates.Num() > 0)
	{
		return mStates.Last();
	}
	return ETileState::None;
}