// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ParticulasComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UParticulasComponent::UParticulasComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // No usamos Tick
	UsesRemaining = MaxUses;
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

	if (UsesRemaining <= 0) return;

	UWorld* World = GetWorld();
	if (!World) return;

	float CurrentTime = World->GetTimeSeconds();

	if (CurrentTime - LastSpawnTime < Cooldown) return;

	AActor* Owner = GetOwner();
	if (!Owner) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ParticleSystem,
		Owner->GetActorLocation(),
		Owner->GetActorRotation()
	);

	LastSpawnTime = CurrentTime;
	UsesRemaining--;
}

