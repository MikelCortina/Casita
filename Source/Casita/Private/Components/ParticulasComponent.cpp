// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ParticulasComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UParticulasComponent::UParticulasComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UParticulasComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UParticulasComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

