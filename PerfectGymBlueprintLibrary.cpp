// Fill out your copyright notice in the Description page of Project Settings.

#include "PerfectGymBlueprintLibrary.h"

#include "Engine.h"

#include "GameFramework/PlayerController.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/OnlineSession.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "Net/OnlineEngineInterface.h"

#include "Engine/LevelStreamingKismet.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"

#include "GameFramework/Volume.h"
#include "Engine/Brush.h"
#include "Engine/BrushBuilder.h"
#include "Components/BrushComponent.h"

#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"

void UPerfectGymBlueprintLibrary::ClientTravelToSession(APlayerController* InPC, ETravelType InTravelType)
{
	UWorld* World = InPC->GetWorld();

	FString URL;
	if (UOnlineEngineInterface::Get()->GetResolvedConnectString(World, GameSessionName, URL))
	{
		if (InPC)
		{
			URL = World->RemovePIEPrefix(URL);
			InPC->ClientTravel(URL, InTravelType);
		}
		else
		{
			//UE_LOG(LogGame, Warning, TEXT("Failed to find local player for controller id %d"), InPC->GetLocalPlayer()->GetControllerId());
		}
	}
	else
	{
		//UE_LOG(LogGame, Warning, TEXT("Failed to resolve session connect string for %s"));
	}
}

void UPerfectGymBlueprintLibrary::GetActorsOfClassFromStreamedLevel(ULevelStreamingKismet* InLevel, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutFoundActors)
{
	OutFoundActors.Reset();

	if (InLevel)
	{
		if (ULevel* LoadedLevel = InLevel->GetLoadedLevel())
		{
			GetActorsOfClassFromULevel(LoadedLevel, ActorClass, OutFoundActors);
		}
	}
}

void UPerfectGymBlueprintLibrary::GetActorsOfClassFromULevel(ULevel* InLevel, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutFoundActors)
{
	OutFoundActors.Reset();
	if (InLevel)
	{
		const TArray<AActor*>& LevelActors = InLevel->Actors;
		for (int32 i = 0; i < LevelActors.Num(); ++i)
		{
			AActor* LevelActor = LevelActors[i];

			if (LevelActor && LevelActor->IsA(ActorClass))
			{
				OutFoundActors.Add(LevelActor);
			}
		}
	}
}

AActor* UPerfectGymBlueprintLibrary::SpawnActorIntoLevel(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, ULevelStreaming* Level, FVector Location, FRotator Rotation)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return nullptr;
	}
	if (!Level) return nullptr;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.OverrideLevel = Level->GetLoadedLevel();

	return World->SpawnActor(ActorClass, &Location, &Rotation, SpawnParameters); 
}

void UPerfectGymBlueprintLibrary::RerunActorConstructionScripts(AActor* InActor)
{
	if (InActor)
	{
		InActor->RerunConstructionScripts();
	}
}
void UPerfectGymBlueprintLibrary::CallActorFunctionByName(UObject * object, FString functionName)
{
	if (!object) return;
	FOutputDeviceNull ar;
	object->CallFunctionByNameWithArguments(functionName.GetCharArray().GetData(), ar, NULL, true);
}
void UPerfectGymBlueprintLibrary::SetLightmapTypeActor(AStaticMeshActor *name, ELightmapType type)
{
	UStaticMeshComponent *component = name->GetStaticMeshComponent();
	component->LightmapType = type;
}

void UPerfectGymBlueprintLibrary::SetUseVertexNormalForHemisphereGatherActor(AStaticMeshActor *name, bool value)
{
	UStaticMeshComponent *component = name->GetStaticMeshComponent();
	component->LightmassSettings.bUseVertexNormalForHemisphereGather = value;
}

void UPerfectGymBlueprintLibrary::SetLightmapChannelsActor(AStaticMeshActor *name, bool channel0, bool channel1, bool channel2)
{
	UStaticMeshComponent *component = name->GetStaticMeshComponent();
	FLightingChannels type;
	type.bChannel0 = channel0;
	type.bChannel1 = channel1;
	type.bChannel2 = channel2;
	component->LightingChannels = type;
}

void UPerfectGymBlueprintLibrary::SetLightMapResolutionActor(AStaticMeshActor *name, int value)
{
	UStaticMeshComponent *component = name->GetStaticMeshComponent();
	component->OverriddenLightMapRes = value;
}

void UPerfectGymBlueprintLibrary::SetLightmapTypeComponent(UStaticMeshComponent *name, ELightmapType type)
{
	name->LightmapType = type;
}

void UPerfectGymBlueprintLibrary::SetUseVertexNormalForHemisphereGatherComponent(UStaticMeshComponent *name, bool value)
{
	name->LightmassSettings.bUseVertexNormalForHemisphereGather = value;
}

void UPerfectGymBlueprintLibrary::SetLightmapChannelsComponent(UStaticMeshComponent *name, bool channel0, bool channel1, bool channel2)
{
	FLightingChannels type;
	type.bChannel0 = channel0;
	type.bChannel1 = channel1;
	type.bChannel2 = channel2;
	name->LightingChannels = type;
}

void UPerfectGymBlueprintLibrary::SetLightMapResolutionComponent(UStaticMeshComponent *name, int value)
{
	name->OverriddenLightMapRes = value;
}

int32 UPerfectGymBlueprintLibrary::GetLightMapResolutionComponent(UStaticMesh *target)
{
	int32 resolution;
	resolution = target->LightMapResolution;
	return resolution;
}

