// Fill out your copyright notice in the Description page of Project Settings.


#include "Panal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/MainPlayer.h"

// Sets default values
APanal::APanal()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APanal::OnOverlapBegin);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    FissureComponent = CreateDefaultSubobject<UFissureComponent>(TEXT("FissureComponent"));
}
void APanal::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (bActivated) return;

    AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
    if (Player)
    {
        // Resetea la gravedad del jugador sin activar sus partículas
        Player->ResetGravity();

        // Explosión de rosas de la grieta
        if (FissureComponent)
        {
            FissureComponent->SpawnParticleExplosion();
        }

        bActivated = true;
        UE_LOG(LogTemp, Warning, TEXT("Panal activada — explosión de rosas"));
    }
}
