// Copyright Epic Games, Inc. All Rights Reserved.

#include "REPuzzleGameMode.h"
#include "REPuzzleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AREPuzzleGameMode::AREPuzzleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
