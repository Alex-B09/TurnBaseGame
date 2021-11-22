// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributes.h"

FGameplayAttribute UCharacterAttributes::DefenseAttribute()
{
    // must match the class name and the attribute name
    static FProperty* property = FindFieldChecked<FProperty>(UCharacterAttributes::StaticClass(), 
                                                             GET_MEMBER_NAME_CHECKED(UCharacterAttributes, Defense));
    return FGameplayAttribute(property);

    // the macro seems to be doing all that boilerplate by itself
}

FGameplayAttribute UCharacterAttributes::HealthAttribute()
{
    static FProperty* property = FindFieldChecked<FProperty>(UCharacterAttributes::StaticClass(),
                                                             GET_MEMBER_NAME_CHECKED(UCharacterAttributes, Health));
    return FGameplayAttribute(property);
}


FGameplayAttribute UCharacterAttributes::DamageToApplyAttribute()
{
    static FProperty* property = FindFieldChecked<FProperty>(UCharacterAttributes::StaticClass(),
        GET_MEMBER_NAME_CHECKED(UCharacterAttributes, DamageToApply));
    return FGameplayAttribute(property);
}

FGameplayAttribute UCharacterAttributes::DamageAttribute()
{
    static FProperty* property = FindFieldChecked<FProperty>(UCharacterAttributes::StaticClass(),
        GET_MEMBER_NAME_CHECKED(UCharacterAttributes, Damage));
    return FGameplayAttribute(property);
}
