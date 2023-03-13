// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Runtime/Core/Public/Async/TaskGraphInterfaces.h>
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/EngineTypes.h"
#include "PerfectGymBlueprintLibrary.generated.h"

class APlayerController;
class ULevelStreamingDynamic;
class ALevelScriptActor;
class ULevelStreaming;

UCLASS()
class VIRORACE_API UPerfectGymBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibrary")
	static void ClientTravelToSession(APlayerController* InPC, ETravelType InTravelType);
	
	UFUNCTION(BlueprintCallable, Category = "BlueprintLibrary")
	static void GetActorsOfClassFromStreamedLevel(ULevelStreamingDynamic* InLevel, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutFoundActors);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibrary")
	static void GetActorsOfClassFromULevel(ULevel* InLevel, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutFoundActors);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibrary", meta = (WorldContext = "WorldContextObject"))
	static AActor* SpawnActorIntoLevel(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, ULevelStreaming* Level, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "BlueprintLibrary")
	static void RerunActorConstructionScripts(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	static void CallActorFunctionByName(UObject* object, FString functionName);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static void SetLightmapTypeActor(AStaticMeshActor *target, ELightmapType type);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static void SetLightMapResolutionActor(AStaticMeshActor *target, int value);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static void SetLightmapChannelsActor(AStaticMeshActor *target, bool channel0, bool channel1, bool channel2);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static void SetUseVertexNormalForHemisphereGatherActor(AStaticMeshActor *target, bool value);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static void SetLightmapTypeComponent(UStaticMeshComponent *target, ELightmapType type);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static void SetLightMapResolutionComponent(UStaticMeshComponent *target, int value);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static void SetLightmapChannelsComponent(UStaticMeshComponent *target, bool channel0, bool channel1, bool channel2);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static void SetUseVertexNormalForHemisphereGatherComponent(UStaticMeshComponent *target, bool value);

	UFUNCTION(BlueprintCallable, Category = "MikiTools")
	static int32 GetLightMapResolutionComponent(UStaticMesh *target);
};
