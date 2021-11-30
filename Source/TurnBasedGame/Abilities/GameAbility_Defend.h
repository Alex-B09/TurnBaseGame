// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilityBase.h"

#include "GameAbility_Defend.generated.h"

// I have done the movement in BP
// this one is going to be in C++ (just to learn)

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UGameAbility_Defend : public UGameAbilityBase
{
	GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
        TSubclassOf<UGameplayEffect> mDefenseEffect;
    
    UPROPERTY()
        bool mIsVisualDone = false;
public:
    UGameAbility_Defend();
    //void Setup(TSubclassOf<UGameplayEffect> defenseEffect);

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UFUNCTION(BlueprintImplementableEvent)
        void HandleVisual();

    UFUNCTION(BlueprintImplementableEvent)
        void DelayEnd();


protected:
    UFUNCTION(BlueprintCallable)
        void VisualDone();
};
