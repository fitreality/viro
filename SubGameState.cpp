// Fill out your copyright notice in the Description page of Project Settings.

#include "SubGameState.h"
#include "ViroRace.h"
#include "UnrealNetwork.h"
#include "Engine/LevelStreamingKismet.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"
#include "PackageName.h"
#include "Engine.h"

ASubGameState::ASubGameState(const FObjectInitializer& InOI) : Super(InOI)
{
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = false;

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickEnabled(false);
	bAllowTickBeforeBeginPlay = false;

	PrevAndCurrentSubStateName.Init(NAME_None, 2);
}

void ASubGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASubGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ASubGameState::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	for (auto it = SubStates.CreateIterator(); it; ++it)
	{
		it->Value->SetOwningGameState(nullptr);
		it->Value->SetOwningSubState(nullptr);
		it->Value->ConditionalBeginDestroy();
	}

	SubStates.Reset();

	Super::EndPlay(EndPlayReason);
}

void ASubGameState::Init(APGGameState* InOwningGameState)
{
	if (HasAuthority())
	{
		this->SetOwningGameState(InOwningGameState);

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.ObjectFlags |= RF_Transient;

		for (auto it = SubStatesTemplates.CreateConstIterator(); it; ++it)
		{
			const FName& SubStateName = it->Key;
			TSubclassOf<ASubGameState> SubStateTemplate = it->Value;
			if (SubStateTemplate)
			{
				ASubGameState* SpawnedSubState = GetWorld()->SpawnActor<ASubGameState>(SubStateTemplate, SpawnInfo);
				SpawnedSubState->SetOwningSubState(this);

				SpawnedSubState->Init(InOwningGameState);

				SubStates.Add(SubStateName, SpawnedSubState);
				SubStatesForReplication.Add(SpawnedSubState);
			}
		}
	}
}

void ASubGameState::OnAfterBeginPlay_Implementation()
{
	for (auto it = SubStates.CreateIterator(); it; ++it)
	{
		it->Value->SetOwningGameState(GetOwningGameState());
		it->Value->OnAfterBeginPlay();
	}
}

void ASubGameState::OnEnter_Implementation()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
}

void ASubGameState::OnExit_Implementation()
{
	if (ASubGameState* currentGameState = GetCurrentSubState())
	{
		currentGameState->OnExit();
	}

	SetActorTickEnabled(false);
	PrimaryActorTick.bCanEverTick = false;
}

APGGameState* ASubGameState::GetOwningGameState()
{
	return OwningGameState;
}

void ASubGameState::SetOwningGameState(APGGameState* InOwningGameState)
{
	OwningGameState = InOwningGameState;
}

ASubGameState* ASubGameState::GetOwningSubState()
{
	return OwningSubState;
}

void ASubGameState::SetOwningSubState(ASubGameState* InOwningSubState)
{
	OwningSubState = InOwningSubState;
}

//Substates
void ASubGameState::ChangeSubState(FName InNewSubStateName)
{
	if (!HasAuthority())
		return;

	FName& PreviousSubStateName = PrevAndCurrentSubStateName[0];
	FName& CurrentSubStateName = PrevAndCurrentSubStateName[1];

	if (CurrentSubStateName != InNewSubStateName)
	{
		PreviousSubStateName = CurrentSubStateName;
		CurrentSubStateName = InNewSubStateName;
		OnRep_SubStateName();
	}
}

ASubGameState* ASubGameState::GetCurrentSubState()
{
	if (ASubGameState** CurrentSubState = SubStates.Find(PrevAndCurrentSubStateName[1]))
	{
		return (*CurrentSubState);
	}
	return nullptr;
}

ASubGameState* ASubGameState::GetSubState(FName SubStateName)
{
	if (ASubGameState** SubState = SubStates.Find(SubStateName))
	{
		return (*SubState);
	}
	return nullptr;
}

void ASubGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASubGameState, SubStatesForReplication, COND_InitialOnly);
	DOREPLIFETIME(ASubGameState, PrevAndCurrentSubStateName);
}

void ASubGameState::OnPostLogin_Implementation(APlayerController* InPlayerController)
{
	for (auto it = SubStates.CreateIterator(); it; ++it)
	{
		it->Value->OnPostLogin(InPlayerController);
	}
}

void ASubGameState::OnLogout_Implementation(APlayerController* InPlayerController)
{
	for (auto it = SubStates.CreateIterator(); it; ++it)
	{
		it->Value->OnLogout(InPlayerController);
	}
}

void ASubGameState::StreamLevels(const TArray<TSoftObjectPtr<UWorld>>& Levels, const TArray<FVector>& Locations, const FOnLevelsLoadedEvent& OnLevelsLoadedDelegate)
{
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return;
	}

	PreviousLevels = LoadedLevels;
	LoadedLevels.Reset();
	NumLevelsLoaded = 0;
	OnAllLevelsLoaded.Clear();
	
	bool bAllSuccess = true;
	ensure(LoadedLevels.Num() == 0);

	for (int32 i = 0; i < Levels.Num(); ++i)
	{
		if (Levels[i].GetAssetName().IsEmpty())
		{
			return;
		}

		bool bSuccess = false;
		ULevelStreamingKismet* LoadedLevel = ULevelStreamingKismet::LoadLevelInstanceBySoftObjectPtr(this, Levels[i], Locations[i], FRotator(0.0f, 0.0f, 0.0f), bSuccess);

		//LoadedLevel->bInitiallyVisible = false;
		//LoadedLevel->bShouldBeVisible = false;
		//LoadedLevel->bShouldBeLoaded = true;

		const FString& LongPackageName = Levels[i].GetLongPackageName();
		const FString ShortPackageName = FPackageName::GetShortName(LongPackageName);
		const FString PackagePath = FPackageName::GetLongPackagePath(LongPackageName);
		FString UniqueLevelPackageName = PackagePath + TEXT("/") + ShortPackageName;
		LoadedLevel->SetWorldAssetByPackageName(FName(*UniqueLevelPackageName));

		if (bSuccess)
		{
			LoadedLevels.Add(LoadedLevel);
		}
		bAllSuccess &= bSuccess;
	}

	if (bAllSuccess)
	{
		if (OnLevelLoadedDelegateHandle.IsValid())
		{
			FWorldDelegates::LevelAddedToWorld.Remove(OnLevelLoadedDelegateHandle);
		}
		OnLevelLoadedDelegateHandle = FWorldDelegates::LevelAddedToWorld.AddUObject(this, &ASubGameState::OnLevelLoaded);
	}
	
	OnAllLevelsLoaded.Add(OnLevelsLoadedDelegate);
}

void ASubGameState::StreamSubLevels(const TArray<FName>& SubLevels, const TArray<FVector>& Locations)
{
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return;
	}

	bool bAllSuccess = true;

	for (int32 i = 0; i < SubLevels.Num(); ++i)
	{
		if (!SubLevels[i].IsValid())
		{
			return;
		}

		bool bSuccess = false;
		ULevelStreamingKismet* LoadedLevel = ULevelStreamingKismet::LoadLevelInstance(this, SubLevels[i].ToString(), Locations[i], FRotator(0.0f, 0.0f, 0.0f), bSuccess);
		//const FString& LongPackageName = SubLevels[i].GetLongPackageName();
		//const FString ShortPackageName = FPackageName::GetShortName(LongPackageName);
		//const FString PackagePath = FPackageName::GetLongPackagePath(LongPackageName);
		//FString UniqueLevelPackageName = PackagePath + TEXT("/") + ShortPackageName;

		//LoadedLevel->bInitiallyVisible = false;
		//LoadedLevel->bShouldBeVisible = false;
		//LoadedLevel->bShouldBeLoaded = true;
		LoadedLevel->SetWorldAssetByPackageName(SubLevels[i]);

		if (bSuccess)
		{
			LoadedLevels.Add(LoadedLevel);
		}
		bAllSuccess &= bSuccess;
	}
}

void ASubGameState::UnloadLevels()
{
	for (int32 i = 0; i < LoadedLevels.Num(); ++i)
	{
		ULevelStreamingKismet* LoadedLevel = LoadedLevels[i];
		LoadedLevel->SetIsRequestingUnloadAndRemoval(true);
	}

	LoadedLevels.Reset();
	NumLevelsLoaded = 0;
}

void ASubGameState::OnLevelLoaded(ULevel* InLevel, UWorld* InWorld)
{
	bool bLevelIsValid = false;
	for (int32 i = 0; i < LoadedLevels.Num(); ++i)
	{
		if (InLevel == LoadedLevels[i]->GetLoadedLevel())
		{
			bLevelIsValid = true;
			break;
		}
	}

	if (bLevelIsValid)
	{
		NumLevelsLoaded++;
		if (NumLevelsLoaded >= LoadedLevels.Num())
		{
			if (UWorld* NewWorld = Cast<UWorld>(InLevel->GetOuter()))
			{
				if (NewWorld->GetStreamingLevels().Num() > 0)
				{
					TArray<FName> SubLevels;
					TArray<FVector> SubLevelsLocations;
					for (int32 i = 0; i < NewWorld->GetStreamingLevels().Num(); ++i)
					{
						FString AssetName = "";
						AssetName.Append(NewWorld->GetStreamingLevels()[i]->GetWorldAsset().GetLongPackageName());
						AssetName = NewWorld->RemovePIEPrefix(AssetName);
						SubLevels.Add(*AssetName);
						SubLevelsLocations.Add(FVector(0.0f, 0.0f, 0.0f));
					}

					StreamSubLevels(SubLevels, SubLevelsLocations);
					return;
				}
			}

			OnAllLevelsLoaded.Broadcast(LoadedLevels);

			for (int32 i = 0; i < PreviousLevels.Num(); ++i)
			{
				ULevelStreamingKismet* PreviousLevel = PreviousLevels[i];
				PreviousLevel->SetIsRequestingUnloadAndRemoval(true);
			}
		}
	}
}

void ASubGameState::OnRep_SubStatesArray()
{
	bool bAllStatesReplicated = true;
	for (int32 i = 0; i < SubStatesForReplication.Num(); ++i)
	{
		bAllStatesReplicated &= SubStatesForReplication[i] != nullptr;
	}

	if (bAllStatesReplicated)
	{
		int32 CurrentSubStateIdx = 0;
		for (auto it = SubStatesTemplates.CreateConstIterator(); it; ++it)
		{
			const FName& SubStateName = it->Key;
			SubStatesForReplication[CurrentSubStateIdx]->SetOwningGameState(GetOwningGameState());
			SubStatesForReplication[CurrentSubStateIdx]->SetOwningSubState(this);
			SubStates.Add(SubStateName, SubStatesForReplication[CurrentSubStateIdx]);

			CurrentSubStateIdx++;
		}

		//OnRep_SubStateName();
	}
}

void ASubGameState::OnRep_SubStateName()
{
	const FName& PreviousSubStateName = PrevAndCurrentSubStateName[0];
	const FName& CurrentSubStateName = PrevAndCurrentSubStateName[1];

	if (ASubGameState** PreviousSubState = SubStates.Find(PreviousSubStateName))
	{
		(*PreviousSubState)->OnExit();
	}
	if (ASubGameState** NewSubState = SubStates.Find(CurrentSubStateName))
	{
		(*NewSubState)->OnEnter();
		OnSubStateChanged(CurrentSubStateName, SubStates[CurrentSubStateName]);
	}
}