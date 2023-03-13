// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "ViroRaceGameModeBase.h"
#include "PGGameState.h"

void AViroRaceGameModeBase::PostLogin(APlayerController* InNewPlayer)
{
	Super::PostLogin(InNewPlayer);

	APGGameState* GameState = GetGameState<APGGameState>();
	if (GameState)
	{
		GameState->AddPlayer(InNewPlayer);
		GameState->OnPostLogin(InNewPlayer);
	}
}

void AViroRaceGameModeBase::Logout(AController* InExiting)
{
	APGGameState* GameState = GetGameState<APGGameState>();
	if (GameState)
	{
		APlayerController* ExitingPC = Cast<APlayerController>(InExiting);

		GameState->OnLogout(ExitingPC);
		GameState->RemovePlayer(ExitingPC);
	}
}