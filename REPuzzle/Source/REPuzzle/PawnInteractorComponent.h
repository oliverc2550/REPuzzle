// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PawnInteractorComponent.generated.h"

class UEnhancedInputComponent;

UCLASS()
class REPUZZLE_API UPawnInteractorComponent : public USphereComponent
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* InteractAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* CycleInteractTargetRightAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* CycleInteractTargetLeftAction;

public:	
	// Sets default values for this component's properties
	UPawnInteractorComponent();

	float GetInteractionRange();

	void SetupInputs( UEnhancedInputComponent* enhancedInputComponent );

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Interact();

	void StopHighLightingInteractTarget();

	void HighLightInteractTarget();

	int32 FindAndCheckIndexOfCurrentInteractTarget();

	void CycleInteractTargetRight();

	void CycleInteractTargetLeft();

	bool IsInteractableActorInFrontOfPlayer( const AActor* otherActor );

	void RemoveInteractableActorsThatArentInFrontOfPlayer();

	void AddUniqueInteractableActorsThatAreInFrontOfPlayerToArray( AActor* otherActor );

	void UpdateInteractableActorsInFrontOfPlayer();

protected:
	struct FTimerHandle InteractOverlapCheckTimer;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0.0f, ClampMax = 512.0f ), Category = "Interaction" )
	float InteractionRange;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0.0, ClampMax = 180.0 ), Category = "Interaction" )
	double MaxInteractAngleInFrontOfPlayer;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Interaction" )
	TArray<AActor*> OverlappedInteractableActorsInFrontOfPlayer;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Interaction" )
	AActor* InteractTarget;
};
