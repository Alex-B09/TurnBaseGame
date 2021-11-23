// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"
#include "Helpers/AbilityHelper.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    mAbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

    //mAttributes = NewObject<UCharacterAttributes>(this);
    mAttributes = CreateDefaultSubobject<UCharacterAttributes>(TEXT("Ability Set"));
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if (mAbilitySystem)
    {
        mAbilitySystem->InitAbilityActorInfo(this, this);
        
        if (mInitialAttributes)
        {
            mAttributes->InitFromMetaDataTable(mInitialAttributes);
        }

        //auto specHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(ability.GetDefaultObject(), 1, 0));

        mAbilitySystem->GiveAbility(FGameplayAbilitySpec(mMovementAbility.GetDefaultObject(), 0));
        mAbilitySystem->GiveAbility(FGameplayAbilitySpec(mAttackAbility.GetDefaultObject(), 0));
        mAbilitySystem->GiveAbility(FGameplayAbilitySpec(mDefendAbility.GetDefaultObject(), 0));
        mAbilitySystem->GiveAbility(FGameplayAbilitySpec(mReceiveDamageAbility.GetDefaultObject(), 0));
    }
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

UAbilitySystemComponent* AGameCharacter::GetAbilitySystemComponent() const
{
    return mAbilitySystem;
}

UCharacterAttributes* AGameCharacter::GetAttributes() const
{
    return mAttributes;
}

void AGameCharacter::MovementStarted()
{
    StartedMovementEvent.Broadcast();
}

void AGameCharacter::MovementFinished()
{
    FinishedMovementEvent.Broadcast();
}
