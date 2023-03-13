// Fill out your copyright notice in the Description page of Project Settings.

#include "PGGameState.h"
#include "SubGameState.h"

#include "UnrealNetwork.h"

APGGameState::APGGameState(const FObjectInitializer& InOI) : Super(InOI)
{
	PrevAndCurrentGameSubStateName.Init(NAME_None, 2);
}

void APGGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APGGameState::BeginPlay()
{
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.ObjectFlags |= RF_Transient;

		for (auto it = GameSubStatesTemplates.CreateConstIterator(); it; ++it)
		{
			const FName& SubStateName = it->Key;
			TSubclassOf<ASubGameState> GameSubStateTemplate = it->Value;
			if (GameSubStateTemplate)
			{
				ASubGameState* SpawnedGameSubState = GetWorld()->SpawnActor<ASubGameState>(GameSubStateTemplate, SpawnInfo);
				SpawnedGameSubState->Init(this);
				GameSubStates.Add(SubStateName, SpawnedGameSubState);

				GameSubStatesForReplication.Add(SpawnedGameSubState);
			}
		}
	}
	
	Super::BeginPlay();
}

void APGGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();

	OnAfterBeginPlay();
}

void APGGameState::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	for (auto it = GameSubStates.CreateIterator(); it; ++it)
	{
		it->Value->ConditionalBeginDestroy();
	}

	GameSubStates.Reset();
	GameSubStatesForReplication.Reset();

	Super::EndPlay(EndPlayReason);
}

void APGGameState::OnAfterBeginPlay_Implementation()
{
	for (auto it = GameSubStates.CreateIterator(); it; ++it)
	{
		it->Value->OnAfterBeginPlay();
	}
}

void APGGameState::ChangeGameSubState(FName InNewSubStateName)
{
	if (!HasAuthority())
		return;

	FName& PreviousSubStateName = PrevAndCurrentGameSubStateName[0];
	FName& CurrentSubStateName = PrevAndCurrentGameSubStateName[1];

	if (CurrentSubStateName != InNewSubStateName)
	{
		PreviousSubStateName = CurrentSubStateName;
		CurrentSubStateName = InNewSubStateName;
		OnRep_GameSubStateName();
	}
}

ASubGameState* APGGameState::GetCurrentGameSubState()
{
	if (ASubGameState** CurrentSubState = GameSubStates.Find(PrevAndCurrentGameSubStateName[1]))
	{
		return (*CurrentSubState);
	}
	return nullptr;
}

ASubGameState* APGGameState::GetGameSubState(FName SubStateName)
{
	if (ASubGameState** SubState = GameSubStates.Find(SubStateName))
	{
		return (*SubState);
	}
	return nullptr;
}

void APGGameState::OnRep_GameSubStatesArray()
{
	bool bAllStatesReplicated = true;
	for (int32 i = 0; i < GameSubStatesForReplication.Num(); ++i)
	{
		bAllStatesReplicated &= GameSubStatesForReplication[i] != nullptr;
	}

	if (bAllStatesReplicated)
	{
		int32 CurrentSubStateIdx = 0;
		for (auto it = GameSubStatesTemplates.CreateConstIterator(); it; ++it)
		{
			const FName& SubStateName = it->Key;
			GameSubStatesForReplication[CurrentSubStateIdx]->SetOwningGameState(this);
			GameSubStates.Add(SubStateName, GameSubStatesForReplication[CurrentSubStateIdx]);
			CurrentSubStateIdx++;
		}

		//OnRep_GameSubStateName();
	}
}

void APGGameState::OnRep_GameSubStateName()
{
	const FName& PreviousSubStateName = PrevAndCurrentGameSubStateName[0];
	const FName& CurrentSubStateName = PrevAndCurrentGameSubStateName[1];

	if (ASubGameState** PreviousSubGameState = GameSubStates.Find(PreviousSubStateName))
	{
		(*PreviousSubGameState)->OnExit();
	}
	if (ASubGameState** NewSubGameState = GameSubStates.Find(CurrentSubStateName))
	{
		(*NewSubGameState)->OnEnter();
		OnGameSubStateChanged(CurrentSubStateName, GameSubStates[CurrentSubStateName]);
	}
}

void APGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APGGameState, GameSubStatesForReplication, COND_InitialOnly);
	DOREPLIFETIME(APGGameState, PrevAndCurrentGameSubStateName);
}

void APGGameState::OnPostLogin_Implementation(APlayerController* InPlayerController)
{
	for (auto it = GameSubStates.CreateIterator(); it; ++it)
	{
		it->Value->OnPostLogin(InPlayerController);
	}
}

void APGGameState::OnLogout_Implementation(APlayerController* InPlayerController)
{
	for (auto it = GameSubStates.CreateIterator(); it; ++it)
	{
		it->Value->OnLogout(InPlayerController);
	}
}

void APGGameState::AddPlayer(APlayerController* InPlayerController)
{
	Players.AddUnique(InPlayerController);
}

void APGGameState::RemovePlayer(APlayerController* InPlayerController)
{
	Players.Remove(InPlayerController);
}

const TArray<APlayerController*>& APGGameState::GetPlayers()
{
	return Players;
}