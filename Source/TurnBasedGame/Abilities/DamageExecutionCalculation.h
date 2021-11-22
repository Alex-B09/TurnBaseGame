// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CharacterAttributes.h"

#include "DamageExecutionCalculation.generated.h"


struct AttributeStruct
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DamageToApply);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

    AttributeStruct()
    {
        // we want the health of the target
        // false is to capture the value when the effect is applied
        // true is when created - most likely in the ExecutionCalculation ctor
        // -- this means that the capture of the value can be delayed when the Execute_Implementation is called (with false)
        DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributes, Health, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributes, DamageToApply, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributes, Defense, Target, false);

        // we want the dmg from source
        DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributes, Damage, Source, true); // TODO - Test this
    }
};

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()


public:
    // the ctor will 
    UDamageExecutionCalculation(const FObjectInitializer& objectInitializer);

    void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
