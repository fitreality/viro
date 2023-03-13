// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Delegates/IDelegateInstance.h"
#include "SubGameState.generated.h"

class APGGameState;
class 	ULevelStreamingDynamic;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLevelsLoadedEvent, const TArray<	ULevelStreamingDynamic*>&, LoadedLevels);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelsLoaded, const TArray<	ULevelStreamingDynamic*>&, LoadedLevels);

UCLASS(notplaceable, BlueprintType, Blueprintable)
class VIRORACE_API ASubGameState : public AInfo
{
	GENERATED_BODY()

public:
	ASubGameState(const FObjectInitializer& InOI);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void Init(APGGameState* InOwningGameState);

	UFUNCTION(BlueprintNativeEvent, Category = "SubGameState")
	void OnAfterBeginPlay();

	UFUNCTION(BlueprintNativeEvent, Category = "SubState")
	void OnEnter();

	UFUNCTION(BlueprintNativeEvent, Category = "SubState")
	void OnExit();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SubState")
	APGGameState* GetOwningGameState();

	void SetOwningGameState(APGGameState* InOwningGameState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SubState")
	ASubGameState* GetOwningSubState();

	void SetOwningSubState(ASubGameState* InOwningSubState);

	//SubStates
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubState")
	TMap<FName, TSubclassOf<ASubGameState>> SubStatesTemplates;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SubState")
	FName InitialSubStateName;

	UFUNCTION(BlueprintCallable, Category = "SubState")
	void ChangeSubState(FName InNewSubStateName);

	UFUNCTION(BlueprintImplementableEvent, Category = "SubState")
	void OnSubStateChanged(const FName& InSubStateName, ASubGameState* InSubState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SubState")
	ASubGameState* GetCurrentSubState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SubState")
	ASubGameState* GetSubState(FName SubStateName);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent, Category = "SubState")
	void OnPostLogin(APlayerController* InPlayerController);

	UFUNCTION(BlueprintNativeEvent, Category = "SubState")
	void OnLogout(APlayerController* InPlayerController);

	//Stream levels
	UFUNCTION(BlueprintCallable, Category = "LevelStreaming")
	void StreamLevels(const TArray<TSoftObjectPtr<UWorld>>& Levels, const TArray<FVector>& Locations, const FOnLevelsLoadedEvent& OnLevelsLoadedDelegate);

	void StreamSubLevels(const TArray<FName>& SubLevels, const TArray<FVector>& Locations);

	UFUNCTION(BlueprintCallable, Category = "LevelStreaming")
	void UnloadLevels();

	FOnLevelsLoaded OnAllLevelsLoaded;

protected:
	UPROPERTY()
	APGGameState* OwningGameState;

	UPROPERTY()
	ASubGameState* OwningSubState;

	UPROPERTY()
	TMap<FName, ASubGameState*> SubStates;

	UPROPERTY(Transient, Replicated, ReplicatedUsing = OnRep_SubStatesArray)
	TArray<ASubGameState*> SubStatesForReplication;

	UFUNCTION()
	virtual void OnRep_SubStatesArray();

	UPROPERTY(Transient, Replicated, ReplicatedUsing = OnRep_SubStateName)
	TArray<FName> PrevAndCurrentSubStateName; //0 prev, 1 current

	UFUNCTION()
	virtual void OnRep_SubStateName();

	//
	UPROPERTY()
	TArray<	ULevelStreamingDynamic*> LoadedLevels;

	UPROPERTY()
	TArray<	ULevelStreamingDynamic*> PreviousLevels;

	int32 NumLevelsLoaded;

	UFUNCTION()
	void OnLevelLoaded(class ULevel* InLevel, class UWorld* InWorld);

	FDelegateHandle OnLevelLoadedDelegateHandle;
};
