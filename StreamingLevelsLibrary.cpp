// Fill out your copyright notice in the Description page of Project Settings.


#include "StreamingLevelsLibrary.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Templates/SharedPointer.h"
#include "Misc/CommandLine.h"

#include "Runtime/Engine/Classes/Engine/LevelStreamingDynamic.h"

class 	ULevelStreamingDynamic;

FLevelStreamInstanceInfo::FLevelStreamInstanceInfo(ULevelStreamingDynamic* LevelInstance)
{
	PackageName = LevelInstance->GetWorldAssetPackageFName();
	PackageNameToLoad = LevelInstance->PackageNameToLoad;
	Location = LevelInstance->LevelTransform.GetLocation();
	Rotation = LevelInstance->LevelTransform.GetRotation().Rotator();
	bShouldBeLoaded = LevelInstance->HasLoadedLevel();
	bShouldBeVisible = LevelInstance->GetShouldBeVisibleFlag();
	bShouldBlockOnLoad = LevelInstance->bShouldBlockOnLoad;
	LODIndex = LevelInstance->GetLevelLODIndex();
};

FLevelStreamInstanceInfo UStreamingLevelsLibrary::GetLevelInstanceInfo(ULevelStreamingDynamic* LevelInstance)
{
	return FLevelStreamInstanceInfo(LevelInstance);
}

void UStreamingLevelsLibrary::AddToStreamingLevels(UObject* WorldContextObject, const FLevelStreamInstanceInfo& LevelInstanceInfo)
{
	bool bResult = true;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);

	if (World != nullptr)
	{
		bool bAlreadyExists = false;

		for (auto StreamingLevel : World->GetStreamingLevels())
		{
			if (StreamingLevel->GetWorldAssetPackageFName() == LevelInstanceInfo.PackageName)
			{
				bAlreadyExists = true;
				// KRIS : Would normally log a warning here! Is there a LogVictory?
				break;
			}
		}

		if (!bAlreadyExists)
		{
			FName PackageName = LevelInstanceInfo.PackageName;

			// For PIE Networking: remap the packagename to our local PIE packagename
			FString PackageNameStr = PackageName.ToString();
			if (GEngine->NetworkRemapPath(World->GetNetDriver(), PackageNameStr, true))
			{
				PackageName = FName(*PackageNameStr);
			}

			GEngine->DelayGarbageCollection();

			// Setup streaming level object that will load specified map
			ULevelStreamingDynamic* StreamingLevel = NewObject<ULevelStreamingDynamic>(World, ULevelStreamingDynamic::StaticClass(), NAME_None, RF_Transient, nullptr);
			StreamingLevel->SetWorldAssetByPackageName(PackageName);
			StreamingLevel->LevelColor = FColor::MakeRandomColor();
			StreamingLevel->SetShouldBeLoaded(LevelInstanceInfo.bShouldBeLoaded);
			StreamingLevel->SetShouldBeVisible(LevelInstanceInfo.bShouldBeVisible);
			StreamingLevel->bShouldBlockOnLoad = LevelInstanceInfo.bShouldBlockOnLoad;
			StreamingLevel->bInitiallyLoaded = true;
			StreamingLevel->bInitiallyVisible = true;

			// Transform
			StreamingLevel->LevelTransform = FTransform(LevelInstanceInfo.Rotation, LevelInstanceInfo.Location);

			// Map to Load
			StreamingLevel->PackageNameToLoad = LevelInstanceInfo.PackageNameToLoad;

			// Add the new level to world.
			World->AddStreamingLevel(StreamingLevel);

			World->FlushLevelStreaming(EFlushLevelStreamingType::Full);
		}
	}
}

void UStreamingLevelsLibrary::RemoveFromStreamingLevels(UObject* WorldContextObject, const FLevelStreamInstanceInfo& LevelInstanceInfo)
{

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);

	// Check if the world exists and we have a level to unload
	if (World != nullptr && !LevelInstanceInfo.PackageName.IsNone())
	{

#if WITH_EDITOR
		// If we are using the editor we will use this lambda to remove the play in editor string
		auto GetCorrectPackageName = [&](FName PackageName) {
			FString PackageNameStr = PackageName.ToString();
			if (GEngine->NetworkRemapPath(World->GetNetDriver(), PackageNameStr, true))
			{
				PackageName = FName(*PackageNameStr);
			}

			return PackageName;
		};
#endif

		// Get the package name that we want to check
		FName PackageNameToCheck = LevelInstanceInfo.PackageName;

#if WITH_EDITOR
		// Remove the play in editor string and client id to be able to use it with replication
		PackageNameToCheck = GetCorrectPackageName(PackageNameToCheck);
#endif

		// Find the level to unload
		for (auto StreamingLevel : World->GetStreamingLevels())
		{

			FName LoadedPackageName = StreamingLevel->GetWorldAssetPackageFName();

#if WITH_EDITOR
			// Remove the play in editor string and client id to be able to use it with replication
			LoadedPackageName = GetCorrectPackageName(LoadedPackageName);
#endif

			// If we find the level unload it and break
			if (PackageNameToCheck == LoadedPackageName)
			{
				// This unload the level
				StreamingLevel->SetShouldBeLoaded(false);
				StreamingLevel->SetShouldBeVisible(false);
				// This removes the level from the streaming level list
				StreamingLevel->SetIsRequestingUnloadAndRemoval(true);
				// Force a refresh of the world
				World->FlushLevelStreaming(EFlushLevelStreamingType::Full);
				break;
			}
		}
	}
}

FServerJsonData UStreamingLevelsLibrary::GetServerConfigurationData(FString pathToJson)
{
	TSharedPtr<FJsonObject> JsonObject;
	FString jsonString;
	FServerJsonData data;

	if (FFileHelper::LoadFileToString(jsonString, *pathToJson))
	{
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(jsonString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			data.TrackGuid = JsonObject->GetStringField(TEXT("TrackGuid"));
			data.bIsRealisticMode = JsonObject->GetBoolField(TEXT("IsRealisticMode"));
			data.MaxNumOfPlayers = JsonObject->GetIntegerField(TEXT("MaxNumOfPlayers"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Deserialize JSON file. Path to the file: %s"), *pathToJson);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file. Path to the file: %s"), *pathToJson);
	}

	return data;
}

FString UStreamingLevelsLibrary::GetCommandLineArgument(FString argument)
{
	FString value;
	if (!FParse::Value(FCommandLine::Get(), *argument, value))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetCommandLineArgument, {0} was not passed at launch."), *argument);
		return FString();
	}
	value = value.Replace(TEXT("="), TEXT(""));
	return value;
}