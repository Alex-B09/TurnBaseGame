#pragma once

#include "CoreMinimal.h"

namespace // anonymous
{
    template <class ElementType, class L>
    TArray<ElementType> GetAll(TArray<ElementType> container, L predicate)
    {
        TArray<ElementType> returnValue;

        // taken from Array.h
        for (ElementType* RESTRICT Data = container.GetData(), *RESTRICT DataEnd = Data + container.ArrayNum; Data != DataEnd; ++Data)
        {
            if (predicate(*Data))
            {
                returnValue.Add(*Data);
            }
        }

        return returnValue;
    }

    template <class ReturnType, class ElementType, class L>
    TArray<ReturnType> Select(TArray<ElementType> container, L predicate)
    {
        TArray<ReturnType> returnValue;

        // taken from Array.h
        for (ElementType* RESTRICT Data = container.GetData(), *RESTRICT DataEnd = Data + container.Num(); Data != DataEnd; ++Data)
        {
            returnValue.Add(predicate(*Data));
        }

        return returnValue;
    }
}