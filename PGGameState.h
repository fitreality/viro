// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PGGameState.generated.h"

class ASubGameState;

UCLASS()
class VIRORACE_API APGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	APGGameState(const FObjectInitializer& InOI);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	virtual void HandleBeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubGameState")
	TMap<FName, TSubclassOf<ASubGameState>> GameSubStatesTemplates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubGameState")
	FName InitialGameSubStateName;

	UFUNCTION(BlueprintNativeEvent, Category = "SubGameState")
	void OnAfterBeginPlay();

	//SubStates
	UFUNCTION(BlueprintCallable, Category = "SubGameState")
	void ChangeGameSubState(FName InNewSubStateName);

	UFUNCTION(BlueprintImplementableEvent, Category = "SubGameState")
	void OnGameSubStateChanged(const FName& InSubStateName, ASubGameState* InGameSubState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SubGameState")
	ASubGameState* GetCurrentGameSubState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SubGameState")
	ASubGameState* GetGameSubState(FName GameSubStateName);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent, Category = "GameState")
	void OnPostLogin(APlayerController* InPlayerController);

	UFUNCTION(BlueprintNativeEvent, Category = "GameState")
	void OnLogout(APlayerController* InPlayerController);

	virtual void AddPlayer(APlayerController* InPlayerController);
	
	virtual void RemovePlayer(APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameState")
	const TArray<APlayerController*>& GetPlayers();

protected:
	UPROPERTY()
	TMap<FName, ASubGameState*> GameSubStates;

	UPROPERTY(Transient, Replicated, ReplicatedUsing = OnRep_GameSubStatesArray)
	TArray<ASubGameState*> GameSubStatesForReplication;

	UFUNCTION()
	virtual void OnRep_GameSubStatesArray();

	UPROPERTY(Transient, Replicated, ReplicatedUsing = OnRep_GameSubStateName)
	TArray<FName> PrevAndCurrentGameSubStateName; //0 prev, 1 current

	UFUNCTION()
	virtual void OnRep_GameSubStateName();

	UPROPERTY()
	TArray<APlayerController*> Players;
};
