// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "DamageEffect.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UDamageEffect : public UGameplayEffect
{
    GENERATED_BODY()

public:
    UDamageEffect(const FObjectInitializer& ObjectInitializer);
};
