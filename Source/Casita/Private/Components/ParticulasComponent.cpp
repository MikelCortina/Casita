// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ParticulasComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UParticulasComponent::UParticulasComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // No usamos Tick
}


// Called when the game starts
void UParticulasComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UParticulasComponent::SpawnParticles()
{
	if (!ParticleSystem) return;

	AActor* Owner = GetOwner();
	if (!Owner) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ParticleSystem,
		Owner->GetActorLocation(),
		Owner->GetActorRotation()
	);
}

