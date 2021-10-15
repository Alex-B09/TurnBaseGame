// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "GridTile.generated.h"

UENUM(BlueprintType)
enum class ETileState : uint8
{
	None,
	Selected,
	SelectedCharacter,
	SelectedForMovement,
	SelectedForAttack,
	SelectedForMagic
};

UCLASS(BlueprintType)
class TURNBASEDGAME_API AGridTile : public AActor
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "GameplayLogic", meta = (DisplayName = "Root"))
		USceneComponent* mScenePosition;

	UPROPERTY(EditAnywhere, Category = "GameplayLogic", meta = (DisplayName = "CharacterPosition"))
		USceneComponent* mCharacterPosition;

    UPROPERTY()
		TArray<ETileState> mStates;

public:	
	// Sets default values for this actor's properties
	AGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void SetToSpell();
	void SetToAttack();
	void SetToMovement();
	void SetToSelection();
	void SetToCharacterSelected();

	void RemoveLastState();
	void RemoveAllState();

    UFUNCTION(BlueprintCallable)
	FTransform GetCharacterPosition() const
	{
		return mCharacterPosition->GetComponentTransform();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
		ETileState GetState() const;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
		void BP_UpdateState();
};
