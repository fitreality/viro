// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ViroRaceGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VIRORACE_API AViroRaceGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:	

	virtual void PostLogin(APlayerController* InNewPlayer) override;

	virtual void Logout(AController* InExiting) override;
	
};
