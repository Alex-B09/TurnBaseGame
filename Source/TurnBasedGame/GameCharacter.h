// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GridTile.h"
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

};
