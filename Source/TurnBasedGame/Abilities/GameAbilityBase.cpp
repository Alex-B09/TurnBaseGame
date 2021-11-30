// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilityBase.h"
#include "../TurnBasedGamePlayerController.h"

void UGameAbilityBase::NotifyControllerEndAction()
{
    // this will be problematic when ai comes into play
    if (auto controller = Cast<ATurnBasedGamePlayerController>(CurrentActorInfo->AvatarActor->GetWorld()->GetFirstPlayerController()))
    {
        controller->FinishActionCharacter();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("UGameAbilityBase::NotifyControllerEndAction - invalid controller"));
    }

}
