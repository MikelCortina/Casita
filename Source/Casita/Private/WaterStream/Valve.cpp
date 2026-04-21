// Fill out your copyright notice in the Description page of Project Settings.

#include "WaterStream/Valve.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/MainPlayer.h"
#include "WaterStream/HardWaterStream.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AValve::AValve()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProximityTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityTrigger"));
    ProximityTrigger->SetupAttachment(RootComponent);
    ProximityTrigger->SetSphereRadius(150.0f);
    ProximityTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

    ProximityTrigger->OnComponentBeginOverlap.AddDynamic(this, &AValve::OnProximityBeginOverlap);
    ProximityTrigger->OnComponentEndOverlap.AddDynamic(this, &AValve::OnProximityEndOverlap);
}

void AValve::BeginPlay()
{
    Super::BeginPlay();

    // Bind de la tecla de interacción al PlayerController
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->InputComponent)
    {
        PC->InputComponent->BindAction(
            InteractInputAction,
            IE_Pressed,
            this,
            &AValve::TryInteract
        );
    }
}

void AValve::OnProximityBeginOverlap(UPrimitiveComponent * OverlappedComp,
    AActor * OtherActor,
    UPrimitiveComponent * OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult & SweepResult)
{
    if (Cast<AMainPlayer>(OtherActor))
    {
        bPlayerInRange = true;
        UE_LOG(LogTemp, Warning, TEXT("Jugador cerca de la llave — pulsa Interact"));
    }
}

void AValve::OnProximityEndOverlap(UPrimitiveComponent * OverlappedComp,
    AActor * OtherActor,
    UPrimitiveComponent * OtherComp,
    int32 OtherBodyIndex)
{
    if (Cast<AMainPlayer>(OtherActor))
    {
        bPlayerInRange = false;
    }
}

void AValve::TryInteract()
{
    if (!bPlayerInRange || bActivated) return;

    bActivated = true;

    if (LinkedWaterStream)
    {
        LinkedWaterStream->DeactivateStream();
        UE_LOG(LogTemp, Warning, TEXT("Llave girada — agua cerrada"));
    }
}
