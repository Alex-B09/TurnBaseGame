#pragma once

#include "CoreMinimal.h"

#include "ControllerActionState.generated.h"

UENUM()
enum class EControllerActionState : uint8
{
    Selecting,
    UiControlled,
    Moving,
    Attacking
};
