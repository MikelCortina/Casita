// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/FissureComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystemInstanceController.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogFissureComponent);

UFissureComponent::UFissureComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFissureComponent::BeginPlay()
{
    Super::BeginPlay();

    // Busca el BoxComponent en el Actor que tiene este componente
    TriggerBox = GetOwner()->FindComponentByClass<UBoxComponent>();

    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(
            this, &UFissureComponent::OnTriggerBeginOverlap);
    }
    else
    {
        UE_LOG(LogFissureComponent, Warning,
            TEXT("FissureComponent: El Actor '%s' no tiene BoxComponent. "
                "Añade uno para que funcione el trigger."),
            *GetOwner()->GetName());
    }
}

void UFissureComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // De momento vacío — aquí puedes añadir lógica por frame si la necesitas
}

void UFissureComponent::OnTriggerBeginOverlap(
    UPrimitiveComponent* OverlapComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // DEBUG temporal
    if (OtherActor)
    {
        UE_LOG(LogFissureComponent, Warning, TEXT("Overlap detectado con: %s"), *OtherActor->GetName());
    }

    if (!OtherActor || !OtherActor->IsA<APawn>() || bOnCooldown)
        return;

    SpawnParticleExplosion();
}

void UFissureComponent::SpawnParticleExplosion()
{
    if (bHasBeenTriggered) return;
    bHasBeenTriggered = true;

    if (!ExplosionEffect) return;

    FVector  SpawnLocation = GetOwner()->GetActorLocation();
    FRotator SpawnRotation = GetOwner()->GetActorRotation();

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        ExplosionEffect,
        SpawnLocation,
        SpawnRotation,
        FVector(EffectScale),
        false, 
        true,
        ENCPoolMethod::None
    );
}
