// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "GridSquare.generated.h"

UCLASS()
class TURNBASEDGAME_API AGridSquare : public AActor
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "GameplayLogic", meta = (DisplayName = "Root"))
		USceneComponent* mScenePosition;

	UPROPERTY(EditAnywhere, Category = "GameplayLogic", meta = (DisplayName = "CharacterPosition"))
		USceneComponent* mCharacterPosition;

public:	
	// Sets default values for this actor's properties
	AGridSquare();

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


protected:
	UFUNCTION(BlueprintImplementableEvent)
		void BP_LightForSpell();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_LightForMovement();
};
