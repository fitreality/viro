// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfigFileLoad.h"
FString UConfigFileLoad::VIROtationFileString;
DEFINE_LOG_CATEGORY(ViroConfigFile);



void UConfigFileLoad::LoadStringFromConfigFile(FString ParameterName, FString &Value, bool &Exist)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		UE_LOG(ViroConfigFile, Log, TEXT("Loading config from: %s"), *ViroStationFile);
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->TryGetStringField(ParameterName, Value))
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Loaded %s: %s"), *ParameterName, *Value);
				Exist = true;
			}
			else
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Parameter %s not found in config file."), *ParameterName);
				Exist = false;
			}
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
			Exist = false;
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
		Exist = false;
	}
}

void UConfigFileLoad::SaveStringToConfigFile(FString ParameterName, FString Value)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&VIROtationFileString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			JsonObject->SetStringField(ParameterName, Value);
			UE_LOG(ViroConfigFile, Log, TEXT("Saved %s: %s"), *ParameterName, *Value);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			FFileHelper::SaveStringToFile(VIROtationFileString, *ViroStationFile);
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
	}
}

void UConfigFileLoad::LoadIntFromConfigFile(FString ParameterName, int &Value, bool &Exist)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		UE_LOG(ViroConfigFile, Log, TEXT("Loading config from: %s"), *ViroStationFile);
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->TryGetNumberField(ParameterName, Value))
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Loaded %s: %d"), *ParameterName, Value);
				Exist = true;
			}
			else
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Parameter %s not found in config file."), *ParameterName);
				Exist = false;
			}
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
			Exist = false;
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
		Exist = false;
	}
}

void UConfigFileLoad::SaveIntToConfigFile(FString ParameterName, int Value)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&VIROtationFileString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			JsonObject->SetNumberField(ParameterName, Value);
			UE_LOG(ViroConfigFile, Log, TEXT("Saved %s: %d"), *ParameterName, Value);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			FFileHelper::SaveStringToFile(VIROtationFileString, *ViroStationFile);
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
	}
}

void UConfigFileLoad::LoadFloatFromConfigFile(FString ParameterName, float &Value, bool &Exist)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		UE_LOG(ViroConfigFile, Log, TEXT("Loading config from: %s"), *ViroStationFile);
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			double ValueD;
			if (JsonObject->TryGetNumberField(ParameterName, ValueD))
			{
				Value = (float)ValueD;
				UE_LOG(ViroConfigFile, Log, TEXT("Loaded %s: %f"), *ParameterName, Value);
				Exist = true;
			}
			else
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Parameter %s not found in config file."), *ParameterName);
				Exist = false;
			}
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
			Exist = false;
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
		Exist = false;
	}
}

void UConfigFileLoad::SaveFloatToConfigFile(FString ParameterName, float Value)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&VIROtationFileString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			JsonObject->SetNumberField(ParameterName, Value);
			UE_LOG(ViroConfigFile, Log, TEXT("Saved %s: %d"), *ParameterName, Value);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			FFileHelper::SaveStringToFile(VIROtationFileString, *ViroStationFile);
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
	}
}

void UConfigFileLoad::LoadBoolFromConfigFile(FString ParameterName, bool &Value, bool &Exist)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		UE_LOG(ViroConfigFile, Log, TEXT("Loading config from: %s"), *ViroStationFile);
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

			if (JsonObject->TryGetBoolField(ParameterName, Value))
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Loaded %s: %s"), *ParameterName, (Value ? TEXT("True") : TEXT("False")));
				Exist = true;
			}
			else
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Parameter %s not found in config file."), *ParameterName);
				Exist = false;
			}
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
			Exist = false;
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
		Exist = false;
	}
}

void UConfigFileLoad::SaveBoolToConfigFile(FString ParameterName, bool Value)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&VIROtationFileString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			JsonObject->SetBoolField(ParameterName, Value);
			UE_LOG(ViroConfigFile, Log, TEXT("Saved %s: %s"), *ParameterName, (Value ? TEXT("True") : TEXT("False")));
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			FFileHelper::SaveStringToFile(VIROtationFileString, *ViroStationFile);
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
	}
}

void UConfigFileLoad::LoadStringsArrayFromConfigFile(FString ParameterName, TArray<FString> &Value, bool &Exist)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ViroStationFile = FPlatformProcess::UserSettingsDir();
	ViroStationFile.Append("FitReality/station.json");
	FPaths::RemoveDuplicateSlashes(ViroStationFile);

	if (PlatformFile.FileExists(*ViroStationFile))
	{
		UE_LOG(ViroConfigFile, Log, TEXT("Loading config from: %s"), *ViroStationFile);
		FFileHelper::LoadFileToString(VIROtationFileString, *ViroStationFile);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VIROtationFileString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (JsonObject->TryGetStringArrayField(ParameterName, Value))
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Loaded %s:"), *ParameterName);
				Exist = true;
			}
			else
			{
				UE_LOG(ViroConfigFile, Log, TEXT("Parameter %s not found in config file."), *ParameterName);
				Exist = false;
			}
		}
		else
		{
			UE_LOG(ViroConfigFile, Log, TEXT("Cannot deserialize file: %s"), *ViroStationFile);
			Exist = false;
		}
	}
	else {
		UE_LOG(ViroConfigFile, Log, TEXT("Cannot find file: %s"), *ViroStationFile);
		Exist = false;
	}
}


