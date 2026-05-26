#include "Grieta.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
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
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    FissureComponent = CreateDefaultSubobject<UFissureComponent>(TEXT("FissureComponent"));
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
        Player->ResetGravity();

        if (FissureComponent)
        {
            FissureComponent->SpawnParticleExplosion();
        }

        bActivated = true;
    }
}