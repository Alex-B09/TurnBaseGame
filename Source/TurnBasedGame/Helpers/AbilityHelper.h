#pragma once

#include "AbilitySystemComponent.h"

namespace AbilityHelper
{
    template <class T>
    bool TryActivateAbilitybyHiearchicalClass(UAbilitySystemComponent *abilitySystem)
    {
        // the biggest problem with this function is the fact that it will stop at the first occurence of T
        // could be a problem in later use

        if (!abilitySystem)
        {
            UE_LOG(LogTemp, Log, TEXT("AbilityHelper::TryActivateAbilityFromParent - invalid ability system"));
            return false;
        }

        auto abilities = abilitySystem->GetActivatableAbilities();

        // dont know why but using IsChildOf did not work
        // i'm gonna use casting here...even though it is not the best option (performance-wise)
        auto ability = abilities.FindByPredicate([=](FGameplayAbilitySpec& ability)
                                              {
                                                  auto castedAbility = Cast<T>(ability.Ability);
                                                  return castedAbility != nullptr;
                                              });
        if (!ability)
        {
            UE_LOG(LogTemp, Log, TEXT("AbilityHelper::TryActivateAbilityFromParent - ability not found"));
            return false;
        }

        if (abilitySystem->TryActivateAbility(ability->Handle))
        {
            UE_LOG(LogTemp,
                   Log,
                   TEXT("AbilityHelper::TryActivateAbilityFromParent - activate ability %s"),
                   *(ability->Ability->GetFName()).ToString());

            return true;
        }
        else // else could be removed but having it makes code a bit easier to read
        {
            UE_LOG(LogTemp, 
                    Log, 
                    TEXT("AbilityHelper::TryActivateAbilityFromParent - could not activate ability %s"), 
                    *(ability->Ability->GetFName()).ToString());

            return false;
        }
    }
}
