// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenVREventDispatcher.h"

// Sets default values
AOpenVREventDispatcher::AOpenVREventDispatcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOpenVREventDispatcher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOpenVREventDispatcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AOpenVREventDispatcher::OnInputFocusReleased.Broadcast();
	

}

