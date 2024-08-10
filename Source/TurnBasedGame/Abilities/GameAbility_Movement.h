// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilityBase.h"
#include "GameAbility_Movement.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UGameAbility_Movement : public UGameAbilityBase
{
    // this class is here as polymorphism only
    //  this way, character will be able to use whatever movement they would want
    //  for example, they could use a flightMovement instead of a walking movement
    GENERATED_BODY()
};
