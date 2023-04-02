// Fill out your copyright notice in the Description page of Project Settings.


#include "LoweringDoor.h"
#include "RotatablePuzzleObject.h"

// Sets default values
ALoweringDoor::ALoweringDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Mesh" ) );
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ALoweringDoor::BeginPlay()
{
	Super::BeginPlay();
}