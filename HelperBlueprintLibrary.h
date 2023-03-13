// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "openvr.h"
#include "ISteamVRPlugin.h"
#include "HelperBlueprintLibrary.generated.h"
class AActor;
class UTexture;
class UTexture2D;
/**
 * 
 */


UCLASS()
class VIRORACE_API UHelperBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable)
		static FString GetCurrentLevelName_bp(AActor* actor);
	
	UFUNCTION(BlueprintCallable)
		static void FadeHMDToColor(float time, float r, float g, float b);
	UFUNCTION(BlueprintCallable)
		static void RestoreHMDImage(float time);

	UFUNCTION(BlueprintCallable, Category = "SteamVR FitReality")
		static void SteamVRHMDProximitySensor(bool &covered);

	UFUNCTION(BlueprintCallable)
		static void BringFront();
	UFUNCTION(BlueprintCallable)
		static void BringBack();

	UFUNCTION(BlueprintCallable)
		static bool SetSkyboxOverride_LatLong(UTexture2D * LatLongSkybox);

	UFUNCTION(BlueprintCallable)
		static void RestoreSkybox();

private:
	static vr::Texture_t CreateOpenVRTexture_t(UTexture * Texture);



};
