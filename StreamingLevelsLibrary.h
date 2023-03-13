// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StreamingLevelsLibrary.generated.h"

USTRUCT(BlueprintType)
struct FLevelStreamInstanceInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		FName PackageName;

	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		FName PackageNameToLoad;

	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		FVector Location;

	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		FRotator Rotation;

	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		uint8 bShouldBeLoaded : 1;

	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		uint8 bShouldBeVisible : 1;

	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		uint8 bShouldBlockOnLoad : 1;

	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		int32 LODIndex;

	FLevelStreamInstanceInfo() {}

	FLevelStreamInstanceInfo(ULevelStreamingDynamic* LevelInstance);

	FString ToString() const
	{
		return FString::Printf(TEXT("PackageName: %s\nPackageNameToLoad:%s\nLocation:%s\nRotation:%s\nbShouldBeLoaded:%s\nbShouldBeVisible:%s\nbShouldBlockOnLoad:%s\nLODIndex:%i")
			, *PackageName.ToString()
			, *PackageNameToLoad.ToString()
			, *Location.ToString()
			, *Rotation.ToString()
			, (bShouldBeLoaded) ? TEXT("True") : TEXT("False")
			, (bShouldBeVisible) ? TEXT("True") : TEXT("False")
			, (bShouldBlockOnLoad) ? TEXT("True") : TEXT("False")
			, LODIndex);
	}
};

USTRUCT(BlueprintType)
struct FLevelStreamInstanceInfoPair
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		FLevelStreamInstanceInfo LevelInfo;
	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		FLevelStreamInstanceInfo GameplayLevelInfo;

	FLevelStreamInstanceInfoPair() {}
};

USTRUCT(BlueprintType)
struct FServerJsonData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		FString TrackGuid;
	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		uint8 bIsRealisticMode : 1; 
	UPROPERTY(Category = "LevelStreaming", BlueprintReadWrite)
		int32 MaxNumOfPlayers;

	FServerJsonData() {}
};

UCLASS()
class VIRORACE_API UStreamingLevelsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
		UFUNCTION(Category = "UStreamingLevelsLibrary", BlueprintCallable)
		static FLevelStreamInstanceInfo GetLevelInstanceInfo(ULevelStreamingDynamic* LevelInstance);

	UFUNCTION(Category = "UStreamingLevelsLibrary", BlueprintCallable, Meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		static void AddToStreamingLevels(UObject* WorldContextObject, const FLevelStreamInstanceInfo& LevelInstanceInfo);

	UFUNCTION(Category = "UStreamingLevelsLibrary", BlueprintCallable, Meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		static void RemoveFromStreamingLevels(UObject* WorldContextObject, const FLevelStreamInstanceInfo& LevelInstanceInfo);

	UFUNCTION(Category = "UStreamingLevelsLibrary", BlueprintCallable, Meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		static FServerJsonData GetServerConfigurationData(FString pathToJson);

	UFUNCTION(Category = "UStreamingLevelsLibrary", BlueprintCallable, Meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		static FString GetCommandLineArgument(FString argument);
};
