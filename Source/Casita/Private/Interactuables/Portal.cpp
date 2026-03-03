// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactuables/Portal.h"
#include "Components/BoxComponent.h"

APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EsteEsNuestroRoot"));
    RootComponent = SceneComponent;

    PortalBoxA = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalBoxA"));
    PortalBoxB = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalBoxB"));

    PortalBoxA->SetupAttachment(RootComponent);
    PortalBoxB->SetupAttachment(RootComponent);

    // Dimensiones básicas (ajusta si quieres)
    PortalBoxA->SetBoxExtent(FVector(50.f, 50.f, 100.f));
    PortalBoxB->SetBoxExtent(FVector(50.f, 50.f, 100.f));

    // Overlap configurado
    PortalBoxA->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PortalBoxA->SetCollisionResponseToAllChannels(ECR_Overlap);
    PortalBoxA->SetGenerateOverlapEvents(true);

    PortalBoxB->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PortalBoxB->SetCollisionResponseToAllChannels(ECR_Overlap);
    PortalBoxB->SetGenerateOverlapEvents(true);
}

void APortal::BeginPlay()
{
    Super::BeginPlay();

    if (PortalBoxA)
    {
        PortalBoxA->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnPortalOverlapA);
    }
    if (PortalBoxB)
    {
        PortalBoxB->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnPortalOverlapB);
    }
}

void APortal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


}

void APortal::ReEnablePortal()
{
    PortalBoxA->SetGenerateOverlapEvents(true);
    PortalBoxB->SetGenerateOverlapEvents(true);
}



void APortal::OnPortalOverlapA(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(APawn::StaticClass()))
    {
        // Deshabilitar temporalmente el portal para evitar "ping-pong"
        PortalBoxA->SetGenerateOverlapEvents(false);
        PortalBoxB->SetGenerateOverlapEvents(false);
        // Aquí puedes implementar la lógica para teletransportar al jugador a través del portal
         // Por ejemplo, podrías cambiar la ubicación del jugador al otro portal
        FVector NewLocation = PortalBoxB->GetComponentLocation(); // Ajusta la posición según sea necesario
        OtherActor->TeleportTo(NewLocation, GetActorRotation());
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APortal::ReEnablePortal, CooldownTime, false);
    }


}

void APortal::OnPortalOverlapB(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(APawn::StaticClass()))
    {
        // Deshabilitar temporalmente el portal para evitar "ping-pong"
        PortalBoxA->SetGenerateOverlapEvents(false);
        PortalBoxB->SetGenerateOverlapEvents(false);
        // Aquí puedes implementar la lógica para teletransportar al jugador a través del portal
         // Por ejemplo, podrías cambiar la ubicación del jugador al otro portal
        FVector NewLocation = PortalBoxA->GetComponentLocation(); // Ajusta la posición según sea necesario
        OtherActor->TeleportTo(NewLocation, GetActorRotation());

        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APortal::ReEnablePortal, CooldownTime, false);
    }


}