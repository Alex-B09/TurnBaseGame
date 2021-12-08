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
    UpdateState(ETileState::Selected);
}

void AGridTile::SetToMovementHighlight()
{
    UpdateState(ETileState::SelectedForMovement);
}

void AGridTile::SetToSpell()
{
    UpdateState(ETileState::SelectedForMagic);
}

void AGridTile::SetToCharacterSelected()
{
	// this is special
	RemoveAllState();
    UpdateState(ETileState::SelectedCharacter);
}

void AGridTile::SetToAttackSelection()
{
    UpdateState(ETileState::SelectedForAttack);
}

void AGridTile::SetToAttackHighlight()
{
    UpdateState(ETileState::HighlightForAttack);
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

void AGridTile::UpdateState(ETileState newState)
{
    mStates.Add(newState);
    BP_UpdateState();
}
