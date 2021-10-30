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
};