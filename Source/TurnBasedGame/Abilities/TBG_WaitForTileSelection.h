// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TBG_WaitForTileSelection.generated.h"



UENUM(BlueprintType)
enum class ETileSelection : uint8
{
    Selected UMETA(DisplayName = "Selected"),
    Cancel UMETA(DisplayName = "Cancelled")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitTileSelect, class AGridTile*, Tile);

/**
 * 
 */
UCLASS()
class TURNBASEDGAME_API UTBG_WaitForTileSelection : public UAbilityTask
{
    GENERATED_BODY()

        // this shows in the bp
        UPROPERTY(BlueprintAssignable)
        FWaitTileSelect OnTileSelect;
    UPROPERTY(BlueprintAssignable)
        FWaitTileSelect OnCancel;

    UFUNCTION()
        void OnTileSelected(class AGridTile* Tile);
    UFUNCTION()
        void OnCancelled();

    // Wait until the ability owner receives damage.
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
        static UTBG_WaitForTileSelection* WaitForTileSlection(UGameplayAbility* OwningAbility, bool TriggerOnce);

    virtual void Activate() override;

protected:
    bool TriggerOnce;

    virtual void OnDestroy(bool AbilityIsEnding) override;
};
