// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageEffect.h"
#include "DamageExecutionCalculation.h"

UDamageEffect::UDamageEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayEffectExecutionDefinition executor;
    executor.CalculationClass = UDamageExecutionCalculation::StaticClass();
    Executions.Add(executor);
}
