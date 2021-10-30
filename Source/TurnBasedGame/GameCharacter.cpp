// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    mAbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (mAbilitySystem)
    {
        mAbilitySystem->InitAbilityActorInfo(this, this);

        //auto specHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(ability.GetDefaultObject(), 1, 0));

        mAbilitySystem->GiveAbility(FGameplayAbilitySpec(mMovementAbility.GetDefaultObject(), 0));
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
