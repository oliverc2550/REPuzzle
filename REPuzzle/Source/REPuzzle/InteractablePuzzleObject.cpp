// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractablePuzzleObject.h"
#include "Camera/CameraComponent.h"
#include "RotatablePuzzleObject.h"

// Sets default values
AInteractablePuzzleObject::AInteractablePuzzleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Mesh" ) );
	RootComponent = StaticMesh;

	PuzzleViewCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "PuzzleCamera" ) );
	PuzzleViewCamera->SetupAttachment( RootComponent ); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
}

// Called when the game starts or when spawned
void AInteractablePuzzleObject::BeginPlay()
{
	Super::BeginPlay();
	
	LinkedPuzzleObject->PuzzleHasBeenSolvedDelegate.AddDynamic( this, &AInteractablePuzzleObject::StopInteracting );
	LinkedPuzzleObject->StopAttemptingPuzzleDelegate.AddUObject( this, &AInteractablePuzzleObject::StopInteracting );
}

void AInteractablePuzzleObject::Interact_Implementation()
{
	if( APlayerController* playerController = GetWorld()->GetFirstPlayerController() )
	{
		playerController->SetViewTargetWithBlend( this, 1.0f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.0f, true );

		LinkedPuzzleObject->EnableInput( playerController );
	}
}

void AInteractablePuzzleObject::StopInteracting()
{
	if( APlayerController* playerController = GetWorld()->GetFirstPlayerController() )
	{
		LinkedPuzzleObject->DisableInput( playerController );
		if( APawn* playerPawn = playerController->GetPawn() )
		{
			playerController->SetViewTargetWithBlend( playerPawn, 1.0f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.0f, true );

			playerPawn->EnableInput( playerController );
		}
	}
}

void AInteractablePuzzleObject::HighlightActor_Implementation()
{
	StaticMesh->SetMaterial( 0, HightLightedMaterial );
}

void AInteractablePuzzleObject::StopHighlightingActor_Implementation()
{
	StaticMesh->SetMaterial( 0, NonHightLightedMaterial );
}

