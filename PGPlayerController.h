// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIRORACE_API APGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BuildInputStack(TArray<UInputComponent*>& InputStack) override;
	
	//virtual void Possess(APawn* InPawn) override;
};
