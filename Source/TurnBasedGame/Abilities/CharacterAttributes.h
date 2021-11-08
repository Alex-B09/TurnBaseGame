// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CharacterAttributes.generated.h"


// predefined ease of life macro for defining attributes -- taken from documentation
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
// im not gonna use this as i want to have a hands-on experience with GAS
// this is left here as documentation for future projects


/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UCharacterAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:	
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities | Stats")
        FGameplayAttributeData Defense;

    // the name of the attribute is super important
    // it must match the attribute define before -- i.e. Defense in this case
    UFUNCTION(Category = "Attribute fonctions")
        FGameplayAttribute DefenseAttribute();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities | Stats")
        FGameplayAttributeData Health;

    UFUNCTION(Category = "Attribute fonctions")
        FGameplayAttribute HealthAttribute();
};
