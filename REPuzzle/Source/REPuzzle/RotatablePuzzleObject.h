// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputActionValue.h"
#include "InputTriggers.h"
#include "RotatablePuzzleObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FPuzzleHasBeenSolved );
DECLARE_MULTICAST_DELEGATE( FStopAttemptingPuzzle );

UCLASS()
class REPUZZLE_API ARotatablePuzzleObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatablePuzzleObject();

protected:
	void SetupPuzzleInput();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool DoesCurrentRotationMatchSolutionRotation();

	void TryToSolvePuzzle();

	void StopSolving();

	FRotator MakeMovementRotatorSteppedOrContinual( double y, double x, double z );

	void RotateMeshComponent( double y, double x, double z );

	void RotateXY( const FInputActionValue& Value );

	void RotateZ( const FInputActionValue& Value );

	void ResetRotationStepBool();

public:
	UPROPERTY( BlueprintAssignable, Category="Puzzle Object")
	FPuzzleHasBeenSolved PuzzleHasBeenSolvedDelegate;

	FStopAttemptingPuzzle StopAttemptingPuzzleDelegate;

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputMappingContext* PuzzleRotationMappingContext;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* StopSolvingAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* CheckPuzzleSolutionAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* RotateXYAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	class UInputAction* RotateZAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.5, ClampMax=10), Category = "Rotation Properties")
	double PuzzleSolutionPercentTolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Properties")
	FRotator PuzzleSolutionRotationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0.5, ClampMax = 10.0 ), Category = "Rotation Properties" )
	double ObjectContinualRotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 1.0, ClampMax = 90.0 ), Category = "Rotation Properties" )
	double ObjectSteppedRotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Properties")
	bool ShouldObjectRotationBeStepped = false;

	bool HasRotationSteppedOnce;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Puzzle Properties" )
	bool CheckIfSolvedAfterEveryMove;
};
