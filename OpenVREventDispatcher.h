// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "openvr.h"
#include "OpenVREventDispatcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSteamVREvent);

UCLASS()
class VIRORACE_API AOpenVREventDispatcher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOpenVREventDispatcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category="OpenVREvent")
		FOnSteamVREvent OnInputFocusReleased;

};
