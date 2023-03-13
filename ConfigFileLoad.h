// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConfigFileLoad.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(ViroConfigFile, Log, All);

/**
 * 
 */
UCLASS()
class VIRORACE_API UConfigFileLoad : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void LoadStringFromConfigFile(FString ParameterName, FString &Value, bool &Exist);
	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void LoadIntFromConfigFile(FString ParameterName, int &Value, bool &Exist);
	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void LoadFloatFromConfigFile(FString ParameterName, float &Value, bool &Exist);
	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void LoadBoolFromConfigFile(FString ParameterName, bool &Value, bool &Exist);

	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void SaveStringToConfigFile(FString ParameterName, FString Value);
	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void SaveIntToConfigFile(FString ParameterName, int Value);
	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void SaveFloatToConfigFile(FString ParameterName, float Value);
	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void SaveBoolToConfigFile(FString ParameterName, bool Value);

	UFUNCTION(BlueprintCallable, Category = "ConfigFile")
		static void LoadStringsArrayFromConfigFile(FString ParameterName, TArray<FString> &Value, bool &Exist);

	static FString VIROtationFileString;
};
