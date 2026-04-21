#include "Grieta.h"
#include "Components/BoxComponent.h"
#include "Player/MainPlayer.h"

AGrieta::AGrieta()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGrieta::OnOverlapBegin);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

void AGrieta::OnOverlapBegin(
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
        // Solo la explosión de la grieta, sin tocar al jugador
        if (FissureComponent)
        {
            FissureComponent->SpawnParticleExplosion();
        }

        bActivated = true;
        UE_LOG(LogTemp, Warning, TEXT("Grieta activada — explosión de rosas"));
    }
}