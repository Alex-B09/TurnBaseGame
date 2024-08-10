// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecutionCalculation.h"
#include "../GameCharacter.h"

UDamageExecutionCalculation::UDamageExecutionCalculation(const FObjectInitializer& objectInitializer)
    :Super(objectInitializer)
{
    AttributeStruct attributes;

    // invalid here signifies that we cannot add modifier in BP (or c++).
    // we just take the value from the attribute set
    
    // to set it to "not modifiable", we need to add it to both the relevant and the invalid
    //      RelevantAttributesToCapture.Add(attributes.HealthDef);
    //      InvalidScopedModifierAttributes.Add(attributes.HealthDef);
    // this basicaly says: we need it (RelevantAttributes) but we dont want to modify it through external modifiers (InvalideScoped)

    RelevantAttributesToCapture.Add(attributes.HealthDef);
    InvalidScopedModifierAttributes.Add(attributes.HealthDef);

    RelevantAttributesToCapture.Add(attributes.DamageToApplyDef);
    InvalidScopedModifierAttributes.Add(attributes.DamageToApplyDef);

    RelevantAttributesToCapture.Add(attributes.DamageDef);
    InvalidScopedModifierAttributes.Add(attributes.DamageDef);

    RelevantAttributesToCapture.Add(attributes.DefenseDef);
    InvalidScopedModifierAttributes.Add(attributes.DefenseDef);
}

void UDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                         FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    AttributeStruct attributes;

    auto& specs = ExecutionParams.GetOwningSpec();

    // not needed but kept here for documentation
    //auto targetAS = ExecutionParams.GetTargetAbilitySystemComponent();
    //auto sourceAS = ExecutionParams.GetSourceAbilitySystemComponent();

    //auto targetActor = Cast<AGameCharacter>(targetAS->GetAvatarActor());
    //auto sourceActor = Cast<AGameCharacter>(sourceAS->GetAvatarActor());

    const auto targetTags = specs.CapturedTargetTags.GetAggregatedTags();
    const auto sourceTags = specs.CapturedSourceTags.GetAggregatedTags();

    // if all is invalid (see ctor), this should not be required
    FAggregatorEvaluateParameters evaluationParams;
    evaluationParams.TargetTags = targetTags;
    evaluationParams.SourceTags = sourceTags;

    float health = 0.f;
    float defense = 0.f;
    float dmg = 0.f;

    // theoricaly, we could get those without the AttemptCalculateCapturedAttributeMagnitude
    // you can just get the ability system, the attributes and be done with it
    //auto sourceActor = Cast<AGameCharacter>(ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor());
    //auto health = sourceActor->GetAttributes()->Health.GetCurrentValue(); // the GetAttribute comes from my framework (not GAS)

    
    // but this is easier and not too costly
    // and, to be honest, it's always better to stay in the GAS framework when working with the GAS
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.HealthDef, evaluationParams, health);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.DefenseDef, evaluationParams, defense);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.DamageDef, evaluationParams, dmg);

    float totalDamage = FMath::Max(0.f, dmg - defense);
    totalDamage = FMath::Min(health, totalDamage); // dont do more then the remaning health

    // setup the outputs
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(attributes.DamageToApplyProperty, EGameplayModOp::Override, totalDamage));
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(attributes.HealthProperty, EGameplayModOp::Additive, -totalDamage));
}
