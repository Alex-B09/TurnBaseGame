#pragma once

#include "CoreMinimal.h"

namespace TagConst
{
    const FName UI_NOACTION(TEXT("UI.NoAction"));
    const FName UI_DEFEND(TEXT("UI.Defend"));
    const FName UI_MOVEMENT(TEXT("UI.Movement"));
    const FName UI_ATTACK(TEXT("UI.Attack"));

    const FName DURATION_NEWTURN(TEXT("Duration.NewTurn"));

    const FName EVENT_DMGRECEIVED(TEXT("Event.DamageReceived"));
    const FName EVENT_DAMAGEFINISH(TEXT("Event.DamageFinish"));
    const FName EVENT_DIE(TEXT("Event.Die"));
    const FName EVENT_WEAPONHIT(TEXT("Event.WeaponHit"));
}
