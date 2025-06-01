// Copyright Epic Games, Inc. All Rights Reserved.

#include "VRPhysicsLabGameMode.h"
#include "VRPhysicsLabCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVRPhysicsLabGameMode::AVRPhysicsLabGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
