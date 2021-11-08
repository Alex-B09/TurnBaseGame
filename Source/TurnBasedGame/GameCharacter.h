// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GridTile.h"
#include "Abilities/CharacterAttributes.h"
#include "Abilities/GameplayAbility.h"



#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameCharacter.generated.h"

UCLASS()
class TURNBASEDGAME_API AGameCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()


    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = true))
        UAbilitySystemComponent* mAbilitySystem;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = true))
        TSubclassOf<UGameplayAbility> mMovementAbility;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Abilities | Data", meta = (AllowPrivateAccess = true))
        UDataTable * mInitialAttributes;

    UPROPERTY()
        UCharacterAttributes* mAttributes;

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
        void Move(AGridTile* currentTile, const TArray<AGridTile*>& tilesToGo);

    UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // attack

    // magic

    // move

    // get stats (hp and whatnot)

    // abilities?



    // events
protected:
    DECLARE_EVENT(AGameCharacter, FCharacterMovement)
    FCharacterMovement StartedMovementEvent;
    FCharacterMovement FinishedMovementEvent;

public:
    FCharacterMovement& OnStartedMovement() // to subscribe to
    {
        return StartedMovementEvent;
    }
    
    FCharacterMovement& OnFinishMovement() // to subscribe to
    {
        return FinishedMovementEvent;
    }


    UFUNCTION(BlueprintCallable)
        void MovementStarted();
    UFUNCTION(BlueprintCallable)
        void MovementFinished();

};
