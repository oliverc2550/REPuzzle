// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatablePuzzleObject.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MathsFunctionLibrary.h"

// Sets default values
ARotatablePuzzleObject::ARotatablePuzzleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Mesh" ) );
	RootComponent = StaticMesh;

	HasRotationSteppedOnce = false;
}

void ARotatablePuzzleObject::SetupPuzzleInput()
{
	InputComponent = NewObject<UEnhancedInputComponent>( this );
	InputComponent->RegisterComponent();

	// Set up action bindings
	if( UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>( InputComponent ) )
	{
		EnhancedInputComponent->BindAction( StopSolvingAction, ETriggerEvent::Triggered, this, &ARotatablePuzzleObject::StopSolving );
		EnhancedInputComponent->BindAction( CheckPuzzleSolutionAction, ETriggerEvent::Completed, this, &ARotatablePuzzleObject::TryToSolvePuzzle );

		EnhancedInputComponent->BindAction( RotateXYAction, ETriggerEvent::Triggered, this, &ARotatablePuzzleObject::RotateXY );
		EnhancedInputComponent->BindAction( RotateXYAction, ETriggerEvent::Completed, this, &ARotatablePuzzleObject::ResetRotationStepBool );
		EnhancedInputComponent->BindAction( RotateZAction, ETriggerEvent::Triggered, this, &ARotatablePuzzleObject::RotateZ );
		EnhancedInputComponent->BindAction( RotateZAction, ETriggerEvent::Completed, this, &ARotatablePuzzleObject::ResetRotationStepBool );
	}

	DisableInput( GetWorld()->GetFirstPlayerController() );
}

// Called when the game starts or when spawned
void ARotatablePuzzleObject::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if( APlayerController* PlayerController = GetWorld()->GetFirstPlayerController() )
	{
		if( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PlayerController->GetLocalPlayer() ) )
		{
			Subsystem->AddMappingContext( PuzzleRotationMappingContext, 1 );
		}
	}

	SetupPuzzleInput();
}

bool ARotatablePuzzleObject::DoesCurrentRotationMatchSolutionRotation()
{
	//Debug
	UE_LOG( LogTemp, Warning, TEXT( "Puzzle Object Current Rotation %s" ), *GetActorRotation().ToString() );
	UE_LOG( LogTemp, Warning, TEXT( "Puzzle Solution Rotation %s" ), *PuzzleSolutionRotationValue.ToString() );
	UE_LOG( LogTemp, Warning, TEXT( "The tolerance is currently %f percent" ), PuzzleSolutionPercentTolerance );
	return Maths::Rotator::EqualsWithinPercentTolerence( GetActorRotation(), PuzzleSolutionRotationValue, PuzzleSolutionPercentTolerance );
}

void ARotatablePuzzleObject::TryToSolvePuzzle()
{
	if( DoesCurrentRotationMatchSolutionRotation() )
	{
		PuzzleHasBeenSolvedDelegate.Broadcast();
	}
}

void ARotatablePuzzleObject::StopSolving()
{
	StopAttemptingPuzzleDelegate.Broadcast();
}

FRotator ARotatablePuzzleObject::MakeMovementRotatorSteppedOrContinual( double y, double x, double z )
{
	if( ShouldObjectRotationBeStepped )
	{
		if( !HasRotationSteppedOnce )
		{
			HasRotationSteppedOnce = true;
			return FRotator( ( y * -ObjectSteppedRotationRate ), ( x * -ObjectSteppedRotationRate ), ( z * ObjectSteppedRotationRate ) );
		}
		else
		{
			return FRotator( 0.0 );
		}
	}
	else
	{
		return FRotator( ( y * -ObjectContinualRotationRate ), ( x * -ObjectContinualRotationRate ), ( z * ObjectContinualRotationRate ) );
	}
}

void ARotatablePuzzleObject::RotateMeshComponent( double y, double x, double z )
{
	FRotator moveRotationAmount = MakeMovementRotatorSteppedOrContinual( y, x, z );

	StaticMesh->SetRelativeRotation( Maths::Rotator::CombineRotators( GetActorRotation(), moveRotationAmount ) );
}

void ARotatablePuzzleObject::RotateXY( const FInputActionValue& Value )
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	RotateMeshComponent( MovementVector.Y, MovementVector.X, 0.0 );

	if( CheckIfSolvedAfterEveryMove )
	{
		TryToSolvePuzzle();
	}
}

void ARotatablePuzzleObject::RotateZ( const FInputActionValue& Value )
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	RotateMeshComponent( 0.0, 0.0, MovementVector.X );

	if( CheckIfSolvedAfterEveryMove )
	{
		TryToSolvePuzzle();
	}
}

void ARotatablePuzzleObject::ResetRotationStepBool()
{
	HasRotationSteppedOnce = false;
}