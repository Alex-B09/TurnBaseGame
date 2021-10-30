// Fill out your copyright notice in the Description page of Project Settings.


#include "TBG_WaitForTileSelection.h"
#include "AbilitySystemComponent.h"
#include <TurnBasedGame/GridTile.h>
#include <TurnBasedGame/TurnBasedGamePlayerController.h>

void UTBG_WaitForTileSelection::OnTileSelected(AGridTile* Tile)
{
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        // broadcast here
        OnTileSelect.Broadcast(Tile);
    }
}

void UTBG_WaitForTileSelection::OnCancelled()
{
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        // broadcast here
        OnCancel.Broadcast(nullptr);
    }
}

void UTBG_WaitForTileSelection::Activate()
{
    auto controller = Cast<ATurnBasedGamePlayerController>(GetWorld()->GetFirstPlayerController());
    auto abilities = Cast<UAbilitySystemComponent>(AbilitySystemComponent);

    if (!controller || !abilities)
    {
        UE_LOG(LogTemp, Log, TEXT("UTBG_WaitForTileSelection::Activate - invalid input"));
        return;
    }

    controller->SetMovementMode();
    controller->OnTileSelect.AddDynamic(this, &UTBG_WaitForTileSelection::OnTileSelected);
    controller->OnCancelled.AddDynamic(this, &UTBG_WaitForTileSelection::OnCancelled);


    //abilities->ReceivedDamage.AddDynamic(this, &UTBG_WaitForTileSelection::OnTileSelected);
}

void UTBG_WaitForTileSelection::OnDestroy(bool AbilityIsEnding)
{
    // unsub to events

    auto controller = Cast<ATurnBasedGamePlayerController>(GetWorld()->GetFirstPlayerController());

    if (!controller)
    {
        UE_LOG(LogTemp, Log, TEXT("UTBG_WaitForTileSelection::OnDestroy - invalid input"));
        return;
    }

    controller->OnTileSelect.RemoveDynamic(this, &UTBG_WaitForTileSelection::OnTileSelected);
    controller->OnCancelled.RemoveDynamic(this, &UTBG_WaitForTileSelection::OnCancelled);

    Super::OnDestroy(AbilityIsEnding);
}


UTBG_WaitForTileSelection* UTBG_WaitForTileSelection::WaitForTileSlection(UGameplayAbility* OwningAbility, bool TriggerOnce)
{
    UTBG_WaitForTileSelection* obj = NewAbilityTask<UTBG_WaitForTileSelection>(OwningAbility);
    obj->TriggerOnce = TriggerOnce;
    return obj;
}
