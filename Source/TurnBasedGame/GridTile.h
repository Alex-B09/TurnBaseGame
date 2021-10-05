// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "GridTile.generated.h"

UCLASS()
class TURNBASEDGAME_API AGridTile : public AActor
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "GameplayLogic", meta = (DisplayName = "Root"))
		USceneComponent* mScenePosition;

	UPROPERTY(EditAnywhere, Category = "GameplayLogic", meta = (DisplayName = "CharacterPosition"))
		USceneComponent* mCharacterPosition;

public:	
	// Sets default values for this actor's properties
	AGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void LightForSpell() {};

	UFUNCTION(BlueprintCallable)
		void LightForMovement() {};


	void Selected(bool isSelected);

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void BP_LightForSpell();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_LightForMovement();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSelection(bool isSelected);

};
