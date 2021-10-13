// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GridTile.h"
#include "GameCharacter.generated.h"

UCLASS()
class TURNBASEDGAME_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()


private:
	// grid position


public:
	// Sets default values for this character's properties
	AGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void Move(AGridTile* currentTile, const TArray<AGridTile*> & tilesToGo);


	// attack

	// magic
	
	// move

	// get stats (hp and whatnot)

	// abilities?

};
