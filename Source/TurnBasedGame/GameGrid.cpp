// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGrid.h"

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