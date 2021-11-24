#pragma once

#include "CoreMinimal.h"

// TODO -   can i make this automaticaly
//          what i hope to do is make a python script (or a plugin) that reads the event in Config/DefaultGameplayTags.ini
//          and generate this file
//          this should be somewhat easy --- famous last words

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
