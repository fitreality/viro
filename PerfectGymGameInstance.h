// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PerfectGymGameInstance.generated.h"

class UANTObject;
//class UUEVRButtons;
/**
 * 
 */
UCLASS()
class VIRORACE_API UPerfectGymGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	//Network Key must be in format 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	UFUNCTION(BlueprintCallable, Category = FitnessEquipment)
	void StartANT(int32 InDeviceNumber, int32 InDeviceType, int32 InChannelType, int32 InTransmissionType = 0, int32 InBaudRate = 57600, int32 InRadioFrequency = 35, FString InNetworkKey = "");

	UFUNCTION(BlueprintCallable, Category = FitnessEquipment)
	void StartANTAsFESlaveController(bool& IsConnected, int32 InDeviceNumber, int32 InDeviceType, int32 InTransmissionType = 0);

	UFUNCTION(BlueprintPure, Category = FitnessEquipment)
	UANTObject* GetAntFESlaveController();

	UFUNCTION(BlueprintCallable, Category = FitnessEquipment)
	void SendTargetPower(int32 InPower);

	UFUNCTION(BlueprintCallable, Category = FitnessEquipment)
	void SetTargetResistance(int32 InResistance);

	UFUNCTION(BlueprintCallable, Category = FitnessEquipment)
	void ResetFitnessEquipmentStats();

	/*UFUNCTION(BlueprintPure, Category = VRButtons)
	UUEVRButtons* GetVRButtons();*/

protected:
	UPROPERTY()
	UANTObject* AntTestObject;

	UPROPERTY()
	UANTObject* AntFESlaveController;

	/*UPROPERTY()
	UUEVRButtons* VRButtonsObject;*/
};
