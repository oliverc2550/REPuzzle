// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnInteractorComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "Interactable.h"
#include "MathsFunctionLibrary.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UPawnInteractorComponent::UPawnInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

float UPawnInteractorComponent::GetInteractionRange()
{
	return InteractionRange;
}

void UPawnInteractorComponent::SetupInputs( UEnhancedInputComponent* enhancedInputComponent )
{
	enhancedInputComponent->BindAction( InteractAction, ETriggerEvent::Completed, this, &UPawnInteractorComponent::Interact );
	enhancedInputComponent->BindAction( CycleInteractTargetRightAction, ETriggerEvent::Completed, this, &UPawnInteractorComponent::CycleInteractTargetRight );
	enhancedInputComponent->BindAction( CycleInteractTargetLeftAction, ETriggerEvent::Completed, this, &UPawnInteractorComponent::CycleInteractTargetLeft );
}

// Called when the game starts
void UPawnInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetWorldTimerManager().SetTimer( InteractOverlapCheckTimer, this, &UPawnInteractorComponent::UpdateInteractableActorsInFrontOfPlayer, 0.5f, true );
}


void UPawnInteractorComponent::Interact()
{
	if( InteractTarget != nullptr && InteractTarget->Implements<UInteractable>() && IsInteractableActorInFrontOfPlayer( InteractTarget ) )
	{
		IInteractable::Execute_Interact( InteractTarget );

		if( APlayerController* PlayerController = GetWorld()->GetFirstPlayerController() )
		{
			GetOwner()->DisableInput( PlayerController );
		}
	}
}

void UPawnInteractorComponent::StopHighLightingInteractTarget()
{
	if( InteractTarget != nullptr && InteractTarget->Implements<UInteractable>() )
	{
		IInteractable::Execute_StopHighlightingActor( InteractTarget );
	}
}

void UPawnInteractorComponent::HighLightInteractTarget()
{
	if( InteractTarget != nullptr && InteractTarget->Implements<UInteractable>() )
	{
		IInteractable::Execute_HighlightActor( InteractTarget );
	}
}

int32 UPawnInteractorComponent::FindAndCheckIndexOfCurrentInteractTarget()
{
	int32 currentTargetIndex = 0;
	if( InteractTarget != nullptr )
	{
		int32 foundIndex = OverlappedInteractableActorsInFrontOfPlayer.IndexOfByKey( InteractTarget );
		if( foundIndex != INDEX_NONE && foundIndex < OverlappedInteractableActorsInFrontOfPlayer.Num() )
		{
			currentTargetIndex = foundIndex;
		}
	}
	return currentTargetIndex;
}

void UPawnInteractorComponent::CycleInteractTargetRight()
{
	int32 currentTargetIndex = FindAndCheckIndexOfCurrentInteractTarget();
	currentTargetIndex++;

	StopHighLightingInteractTarget();

	if( currentTargetIndex < OverlappedInteractableActorsInFrontOfPlayer.Num() )
	{
		InteractTarget = OverlappedInteractableActorsInFrontOfPlayer[ currentTargetIndex ];
	}
	else
	{
		InteractTarget = OverlappedInteractableActorsInFrontOfPlayer[ 0 ];
	}

	HighLightInteractTarget();
}

void UPawnInteractorComponent::CycleInteractTargetLeft()
{
	int32 currentTargetIndex = FindAndCheckIndexOfCurrentInteractTarget();
	currentTargetIndex--;

	StopHighLightingInteractTarget();

	if( currentTargetIndex >= 0 )
	{
		InteractTarget = OverlappedInteractableActorsInFrontOfPlayer[ currentTargetIndex ];
	}
	else
	{
		int32 arrayEndIndex = OverlappedInteractableActorsInFrontOfPlayer.Num() - 1;
		InteractTarget = OverlappedInteractableActorsInFrontOfPlayer[ arrayEndIndex ];
	}

	HighLightInteractTarget();
}

bool UPawnInteractorComponent::IsInteractableActorInFrontOfPlayer( const AActor* otherActor )
{
	FVector unitDirection = Maths::Vector::CalculateUnitDirectionVectorNormalised( GetOwner()->GetActorLocation(), otherActor->GetActorLocation() );
	double dotProduct = FVector::DotProduct( GetOwner()->GetActorForwardVector(), unitDirection );
	double angleInDegrees = FMath::RadiansToDegrees( FMath::Acos( dotProduct ) );
	return angleInDegrees < MaxInteractAngleInFrontOfPlayer;
}

void UPawnInteractorComponent::RemoveInteractableActorsThatArentInFrontOfPlayer()
{
	TArray<AActor*> copyArray;
	copyArray.Append( OverlappedInteractableActorsInFrontOfPlayer );
	for( AActor* interactableActor : copyArray )
	{
		if( !IsInteractableActorInFrontOfPlayer( interactableActor ) )
		{
			OverlappedInteractableActorsInFrontOfPlayer.Remove( interactableActor );
		}
	}

	if( OverlappedInteractableActorsInFrontOfPlayer.Num() == 0 && InteractTarget != nullptr )
	{
		StopHighLightingInteractTarget();
		InteractTarget = nullptr;
	}
}

void UPawnInteractorComponent::AddUniqueInteractableActorsThatAreInFrontOfPlayerToArray( AActor* otherActor )
{
	if( otherActor->Implements<UInteractable>() && IsInteractableActorInFrontOfPlayer( otherActor ) && OverlappedInteractableActorsInFrontOfPlayer.Find( otherActor ) == INDEX_NONE )
	{
		OverlappedInteractableActorsInFrontOfPlayer.Emplace( otherActor );
	}
}

void UPawnInteractorComponent::UpdateInteractableActorsInFrontOfPlayer()
{
	RemoveInteractableActorsThatArentInFrontOfPlayer();

	TArray<AActor*> overlappedActors;
	GetOverlappingActors( overlappedActors );

	for( AActor* otherActor : overlappedActors )
	{
		AddUniqueInteractableActorsThatAreInFrontOfPlayerToArray( otherActor );
	}

	if( OverlappedInteractableActorsInFrontOfPlayer.Num() > 0 && InteractTarget == nullptr )
	{
		InteractTarget = OverlappedInteractableActorsInFrontOfPlayer[ 0 ];
		HighLightInteractTarget();
	}
}


