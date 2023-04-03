// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractablePuzzleObject.generated.h"

UCLASS()
class REPUZZLE_API AInteractablePuzzleObject : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractablePuzzleObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

	UFUNCTION()
	virtual void StopInteracting() override;

	virtual void HighlightActor_Implementation() override;

	virtual void StopHighlightingActor_Implementation() override;

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	class UCameraComponent* PuzzleViewCamera;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Materials" )
	class UMaterialInstance* NonHightLightedMaterial;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Materials" )
	class UMaterialInstance* HightLightedMaterial;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Puzzle Object" )
	class ARotatablePuzzleObject* LinkedPuzzleObject;

};
