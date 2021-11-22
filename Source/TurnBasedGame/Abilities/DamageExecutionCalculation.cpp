// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecutionCalculation.h"
#include "../GameCharacter.h"

UDamageExecutionCalculation::UDamageExecutionCalculation(const FObjectInitializer& objectInitializer)
    :Super(objectInitializer)
{
    AttributeStruct attributes;

    // invalid here signifies that we cannot add modifier in BP (or c++).
    // we just take the value from the attribute set
    //InvalidScopedModifierAttributes.Add(attributes.HealthDef);

    // for some reason, the InvalidScopedModifierAttributes did not work...TODO check that later
    RelevantAttributesToCapture.Add(attributes.HealthDef);
    RelevantAttributesToCapture.Add(attributes.DamageToApplyDef);
    RelevantAttributesToCapture.Add(attributes.DefenseDef);

    // we let the ability to add modifier -- i.e it is available in BP
    RelevantAttributesToCapture.Add(attributes.DamageDef);
}

void UDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                         FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    AttributeStruct attributes;

    auto& specs = ExecutionParams.GetOwningSpec();
    auto targetAS = ExecutionParams.GetTargetAbilitySystemComponent();
    auto sourceAS = ExecutionParams.GetSourceAbilitySystemComponent();

    auto targetActor = Cast<AGameCharacter>(targetAS->GetAvatarActor());
    auto sourceActor = Cast<AGameCharacter>(sourceAS->GetAvatarActor());

    const auto targetTags = specs.CapturedTargetTags.GetAggregatedTags();
    const auto sourceTags = specs.CapturedSourceTags.GetAggregatedTags();

    // if all is invalid (see ctor), this should not be required
    FAggregatorEvaluateParameters evaluationParams;
    evaluationParams.TargetTags = targetTags;
    evaluationParams.SourceTags = sourceTags;

    auto test1 = GetAttributeCaptureDefinitions();

    float health = 0.f;
    float defense = 0.f;
    float dmg = 0.f;

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.HealthDef, evaluationParams, health);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.DefenseDef, evaluationParams, defense);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(attributes.DamageDef, evaluationParams, dmg);

    float totalDamage = FMath::Max(0.f, dmg - defense);
    totalDamage = FMath::Min(health, totalDamage); // dont do more then the remaning health

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(attributes.DamageToApplyProperty, EGameplayModOp::Override, totalDamage));
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(attributes.HealthProperty, EGameplayModOp::Additive, -totalDamage));
}
