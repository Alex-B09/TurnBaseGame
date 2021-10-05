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

void AGridTile::Selected(bool isSelected)
{
	OnSelection(isSelected);
}

// Called every frame
//void AGridTile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

