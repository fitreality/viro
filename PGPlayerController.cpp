// Fill out your copyright notice in the Description page of Project Settings.

#include "PGPlayerController.h"
#include "GameFramework/Pawn.h"

void APGPlayerController::BuildInputStack(TArray<UInputComponent*>& InputStack)
{
	// Controlled pawn gets last dibs on the input stack
	APawn* ControlledPawn = GetPawnOrSpectator();
	if (ControlledPawn)
	{
		if (ControlledPawn->InputEnabled())
		{
			// Get the explicit input component that is created upon Pawn possession. This one gets last dibs.
			if (ControlledPawn->InputComponent)
			{
				InputStack.Push(ControlledPawn->InputComponent);
			}

			// See if there is another InputComponent that was added to the Pawn's components array (possibly by script).
			for (UActorComponent* ActorComponent : ControlledPawn->GetComponents())
			{
				UInputComponent* PawnInputComponent = Cast<UInputComponent>(ActorComponent);
				if (PawnInputComponent && PawnInputComponent != ControlledPawn->InputComponent)
				{
					InputStack.Push(PawnInputComponent);
				}
			}
		}
	}

	//HACK, Levels in world are not properly deleted, so don't push them on input stack
	/*// LevelScriptActors are put on the stack next
	for (ULevel* Level : GetWorld()->GetLevels())
	{
		ALevelScriptActor* ScriptActor = Level->GetLevelScriptActor();
		if (ScriptActor)
		{
			if (ScriptActor->InputEnabled() && ScriptActor->InputComponent)
			{
				InputStack.Push(ScriptActor->InputComponent);
			}
		}
	}*/

	if (InputEnabled())
	{
		InputStack.Push(InputComponent);
	}

	// Components pushed on to the stack get priority
	for (int32 Idx = 0; Idx < CurrentInputStack.Num(); ++Idx)
	{
		UInputComponent* IC = CurrentInputStack[Idx].Get();
		if (IC)
		{
			InputStack.Push(IC);
		}
		else
		{
			CurrentInputStack.RemoveAt(Idx--);
		}
	}
}

/*void APGPlayerController::Possess(APawn* InPawn)
{
	if (InPawn != NULL)
	{
		if (GetPawn() && GetPawn() != InPawn)
		{
			UnPossess();
		}

		if (InPawn->Controller != NULL)
		{
			InPawn->Controller->UnPossess();
		}

		InPawn->PossessedBy(this);
		SetPawn(InPawn);

		// update rotation to match possessed pawn's rotation
		SetControlRotation(InPawn->GetActorRotation());

		InPawn->Restart();
	}
}*/