// Fill out your copyright notice in the Description page of Project Settings.

#include "PerfectGymGameInstance.h"
#include "ANTObject.h"
//#include "UEVRButtons.h"

void UPerfectGymGameInstance::Init()
{
	AntTestObject = NewObject<UANTObject>(this);
	AntFESlaveController = NewObject<UANTObject>(this);

	//VRButtonsObject = NewObject<UUEVRButtons>(this);
}

void UPerfectGymGameInstance::Shutdown()
{
	/*if (VRButtonsObject)
	{
		VRButtonsObject->End();
	}*/
}

void UPerfectGymGameInstance::StartANT(int32 InDeviceNumber, int32 InChannelType, int32 InDeviceType, int32 InTransmissionType, int32 InBaudRate, int32 InRadioFrequency, FString InNetworkKey)
{
	AntTestObject->Init(InDeviceNumber, InChannelType, InDeviceType, InTransmissionType, InBaudRate, InRadioFrequency, InNetworkKey);
	AntTestObject->Start();
}

void UPerfectGymGameInstance::StartANTAsFESlaveController(bool& IsConnected, int32 InDeviceNumber, int32 InDeviceType, int32 InTransmissionType)
{
	IsConnected = false;
	FString AntPlusNetworkKey = "0xB9,0xA5,0x21,0xFB,0xBD,0x72,0xC3,0x45"; //Taken from https://www.thisisant.com/developer/ant-plus/ant-plus-basics/network-keys

	IsConnected = AntFESlaveController->Init(InDeviceNumber, 1, InDeviceType, InTransmissionType, 57600, 57, AntPlusNetworkKey);
	AntFESlaveController->Start();
	
}

void UPerfectGymGameInstance::SendTargetPower(int32 InPower)
{
	InPower *= 4;
	if (InPower >= 0 && InPower <= 16000)
	{
		AntFESlaveController->SendControlMessage(0x31, InPower);
	}
}

void UPerfectGymGameInstance::SetTargetResistance(int32 InResistance)
{
	InResistance *= 2;
	if (InResistance >= 0 && InResistance <= 200)
	{
		AntFESlaveController->SendControlMessageBasicResistance(0x30, InResistance);
	}
}

UANTObject* UPerfectGymGameInstance::GetAntFESlaveController()
{
	return AntFESlaveController;
}

void UPerfectGymGameInstance::ResetFitnessEquipmentStats()
{
	AntFESlaveController->ResetFitnessEquipmentStats();
}

/*UUEVRButtons* UPerfectGymGameInstance::GetVRButtons()
{
	return VRButtonsObject;
}*/