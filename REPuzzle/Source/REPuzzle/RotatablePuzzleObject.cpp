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
	StaticMesh->SetMaterial( 0, NonHightLightedMaterial );

}

//TODO: Fix Stepped Rotation
//ETriggerEvent ARotatablePuzzleObject::AdjustTriggerEventIfStepped()
//{
//	if( ShouldObjectRotationBeStepped )
//	{
//		return ETriggerEvent::Completed;
//	}
//	else
//	{
//		return ETriggerEvent::Triggered;
//	}
//}

void ARotatablePuzzleObject::SetupPuzzleInput()
{
	InputComponent = NewObject<UEnhancedInputComponent>( this );
	InputComponent->RegisterComponent();

	// Set up action bindings
	if( UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>( InputComponent ) )
	{
		EnhancedInputComponent->BindAction( StopInteractingAction, ETriggerEvent::Triggered, this, &ARotatablePuzzleObject::StopInteracting );
		EnhancedInputComponent->BindAction( CheckPuzzleSolutionAction, ETriggerEvent::Completed, this, &ARotatablePuzzleObject::SolvePuzzle );

		EnhancedInputComponent->BindAction( RotateXYAction, ETriggerEvent::Triggered, this, &ARotatablePuzzleObject::RotateXY );
		EnhancedInputComponent->BindAction( RotateZAction, ETriggerEvent::Triggered, this, &ARotatablePuzzleObject::RotateZ );
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

void ARotatablePuzzleObject::SolvePuzzle()
{
	if( DoesCurrentRotationMatchSolutionRotation() )
	{
		PuzzleHasBeenSolvedDelegate.Broadcast();
		StopInteracting();
	}
}

void ARotatablePuzzleObject::Interact_Implementation()
{
	if( APlayerController* playerController = GetWorld()->GetFirstPlayerController() )
	{
		EnableInput( playerController );
	}
}

void ARotatablePuzzleObject::StopInteracting()
{
	if( APlayerController* playerController = GetWorld()->GetFirstPlayerController() )
	{
		DisableInput( playerController );
		if( APawn* playerPawn = playerController->GetPawn() )
		{
			playerPawn->EnableInput( playerController );
		}
	}
}

void ARotatablePuzzleObject::HighlightActor_Implementation()
{
	StaticMesh->SetMaterial(0, HightLightedMaterial);
}

void ARotatablePuzzleObject::StopHighlightingActor_Implementation()
{
	StaticMesh->SetMaterial( 0, NonHightLightedMaterial );
}

FRotator ARotatablePuzzleObject::MakeMovementRotatorSteppedOrContinual( double y, double x, double z )
{
	//TODO: Fix Stepped Rotation
	//if( ShouldObjectRotationBeStepped )
	//{
	//	//Due to Stepped Rotation using ETriggerEvent::Completed any passed in y, x or z values will be zero. Need to figure out how to get proper values.
	//	return FRotator( ( y * -ObjectSteppedRotationRate ), ( x * -ObjectSteppedRotationRate ), ( z * ObjectSteppedRotationRate ) );
	//}
	return FRotator( ( y * -ObjectContinualRotationRate ), ( x * -ObjectContinualRotationRate ), ( z * ObjectContinualRotationRate ) );
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
		SolvePuzzle();
	}
}

void ARotatablePuzzleObject::RotateZ( const FInputActionValue& Value )
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	RotateMeshComponent( 0.0, 0.0, MovementVector.X );

	if( CheckIfSolvedAfterEveryMove )
	{
		SolvePuzzle();
	}
}