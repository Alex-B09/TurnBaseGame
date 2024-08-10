#pragma once

#include "CoreMinimal.h"
#include "GridPosition.generated.h"

USTRUCT(BlueprintType)
struct FGridPosition
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "X"))
        int mPosX;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Y"))
        int mPosY;

    // this should not be required...but i dont trust UE struct system much
    FGridPosition& operator=(const FGridPosition& other)
    {
        mPosX = other.mPosX;
        mPosY = other.mPosY;

        return *this;
    }
};