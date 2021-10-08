// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedGameGameMode.h"
#include "TurnBasedGamePlayerController.h"
#include "UObject/ConstructorHelpers.h"

ATurnBasedGameGameMode::ATurnBasedGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATurnBasedGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	//if (PlayerPawnBPClass.Class != nullptr)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}