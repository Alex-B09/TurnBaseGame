// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"

#include "Subsystems/WorldSubsystem.h"

#include "AbilitySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UAbilitySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
private: // unuseful but i like it


public:
    void RemoveDefendAbility(TArray<IAbilitySystemInterface*> characters);
};
