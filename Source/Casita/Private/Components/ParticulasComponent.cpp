#include "Components/ParticulasComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UParticulasComponent::UParticulasComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    UsesRemaining = MaxUses;
}

void UParticulasComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UParticulasComponent::ResetUses()
{
    UsesRemaining = MaxUses;
}

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