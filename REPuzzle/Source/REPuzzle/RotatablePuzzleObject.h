// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InputActionValue.h"
#include "InputTriggers.h"
#include "RotatablePuzzleObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FPuzzleHasBeenSolved );

UCLASS()
class REPUZZLE_API ARotatablePuzzleObject : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatablePuzzleObject();

protected:
	//TODO: Fix Stepped Rotation
	//ETriggerEvent AdjustTriggerEventIfStepped();

	void SetupPuzzleInput();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool DoesCurrentRotationMatchSolutionRotation();

	void SolvePuzzle();

	virtual void Interact_Implementation() override;

	virtual void StopInteracting() override;

	virtual void HighlightActor_Implementation() override;

	virtual void StopHighlightingActor_Implementation() override;

	FRotator MakeMovementRotatorSteppedOrContinual( double y, double x, double z );

	void RotateMeshComponent( double y, double x, double z );

	void RotateXY( const FInputActionValue& Value );

	void RotateZ( const FInputActionValue& Value );

public:
	UPROPERTY( BlueprintAssignable, Category="Puzzle Object")
	FPuzzleHasBeenSolved PuzzleHasBeenSolvedDelegate;

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputMappingContext* PuzzleRotationMappingContext;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* StopInteractingAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* CheckPuzzleSolutionAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* RotateXYAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* RotateZAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Materials" )
	class UMaterialInstance* NonHightLightedMaterial;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Materials" )
	class UMaterialInstance* HightLightedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.5, ClampMax=10), Category = "Rotation Properties")
	double PuzzleSolutionPercentTolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Properties")
	FRotator PuzzleSolutionRotationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0.5, ClampMax = 10.0 ), Category = "Rotation Properties" )
	double ObjectContinualRotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 1.0, ClampMax = 90.0 ), Category = "Rotation Properties" )
	double ObjectSteppedRotationRate;

	//TODO: Solve how to get input value from ETriggerEvent::Completed Bindings
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Properties")
	//bool ShouldObjectRotationBeStepped = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Puzzle Properties" )
	bool CheckIfSolvedAfterEveryMove;

};
