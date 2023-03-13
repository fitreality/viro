// Fill out your copyright notice in the Description page of Project Settings.

#include "HelperBlueprintLibrary.h"
#include "GameFramework/Actor.h"

#include "Engine/Texture2D.h"
#include "RenderUtils.h"
#include "IXRTrackingSystem.h"
#include "IHeadMountedDisplay.h"

FString UHelperBlueprintLibrary::GetCurrentLevelName_bp(AActor * actor)
{
	if (!actor) return FString();
	auto name = actor->GetLevel()->GetOuter()->GetName();
	return name;
}

void UHelperBlueprintLibrary::FadeHMDToColor(float time, float r, float g, float b)
{
	auto compositor = vr::VRCompositor();
	if (!compositor) return;
	/*UTexture2D* text = nullptr;
	CreateOpenVRTexture_t(text);*/
	//compositor->SetSkyboxOverride()
	compositor->CompositorBringToFront();
	compositor->FadeToColor(time, r, g, b, 1.0);
	
}

void UHelperBlueprintLibrary::SteamVRHMDProximitySensor(bool &covered)
{
	covered = false;
	vr::IVRSystem * VRSystem = ISteamVRPlugin::Get().GetVRSystem();
	//level = VRSystem->GetTrackedDeviceActivityLevel(vr::k_unTrackedDeviceIndex_Hmd);
	auto compositor = vr::VRCompositor();
	//vr::VRControllerState_t::ulButtonPressed
	vr::VRControllerState_t state;
	VRSystem->GetControllerState(0, &state, sizeof(state));
	int status = VRSystem->GetTrackedDeviceActivityLevel(0);
	int level = state.ulButtonPressed & (1UL << ((int)vr::EVRButtonId::k_EButton_ProximitySensor));
	if (level != 0 || status != 0)
	{
		covered = true;
	}
}

void UHelperBlueprintLibrary::RestoreHMDImage(float time)
{
	auto compositor = vr::VRCompositor();
	if (!compositor) return;
	compositor->FadeToColor(time, 0.0, 0.0, 0.0, 0.0);
	compositor->FadeGrid(time, true);
}

void UHelperBlueprintLibrary::BringFront()
{
	auto compositor = vr::VRCompositor();
	if (!compositor) return;
	compositor->FadeToColor(1.0f, 0.0, 0.0, 0.0, 0.0);
	compositor->FadeGrid(1.0f, false);
}

void UHelperBlueprintLibrary::BringBack()
{
	auto compositor = vr::VRCompositor();
	if (!compositor) return;
	compositor->CompositorBringToFront();
	compositor->FadeToColor(1.0f, 0.0, 0.0, 0.0, 0.0);
	compositor->FadeGrid(1.0f, true);
}

bool UHelperBlueprintLibrary::SetSkyboxOverride_LatLong(UTexture2D * LatLongSkybox)
{
	EPixelFormat format = LatLongSkybox->GetPixelFormat();
	UE_LOG(LogTemp, Warning, TEXT("Pixel format is: %d"), format);

	if (!LatLongSkybox)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bad texture passed in to SetSkyBoxOverride"));
		return false;
	}

	vr::HmdError HmdErr;
	vr::IVRCompositor * VRCompositor = (vr::IVRCompositor*)vr::VR_GetGenericInterface(vr::IVRCompositor_Version, &HmdErr);

	if (!VRCompositor)
	{
		UE_LOG(LogTemp, Warning, TEXT("VR Compositor InterfaceErrorCode %i"), (int32)HmdErr);
		return false;
	}

	vr::Texture_t Texture;

	Texture = CreateOpenVRTexture_t(LatLongSkybox);

	vr::EVRCompositorError CompositorError;
	CompositorError = VRCompositor->SetSkyboxOverride(&Texture, 1);

	if (CompositorError != vr::VRCompositorError_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("VR Compositor Error %i"), (int32)CompositorError);
		return false;
	}

	return true;
}

void UHelperBlueprintLibrary::RestoreSkybox()
{
	auto compositor = vr::VRCompositor();
	compositor->ClearSkyboxOverride();
}

vr::Texture_t UHelperBlueprintLibrary::CreateOpenVRTexture_t(UTexture * Texture)
{
	vr::Texture_t VRTexture;

	if (Texture)
		VRTexture.handle = Texture->Resource->TextureRHI->GetNativeResource();
	else
		VRTexture.handle = NULL;

#if PLATFORM_LINUX
#if STEAMVR_USE_VULKAN_RHI
	VRTexture.eType = vr::TextureType_Vulkan;
#else
	VRTexture.eType = vr::TextureType_OpenGL;
#endif
#else
	VRTexture.eType = vr::TextureType_DirectX;
#endif
	VRTexture.eColorSpace = vr::ColorSpace_Auto;

	return VRTexture;
}
