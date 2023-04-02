// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REPUZZLE_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void Interact();

	virtual void StopInteracting() {};

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void HighlightActor();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void StopHighlightingActor();
};
