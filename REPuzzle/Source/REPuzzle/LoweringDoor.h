// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoweringDoor.generated.h"

UCLASS()
class REPUZZLE_API ALoweringDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoweringDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LowerDoor();

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Puzzle Object" )
	class ARotatablePuzzleObject* LinkedPuzzleObject;
};
