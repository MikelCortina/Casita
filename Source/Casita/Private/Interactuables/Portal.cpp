// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactuables/Portal.h"
#include "Components/BoxComponent.h"

// Sets default values
APortal::APortal()
{
        PrimaryActorTick.bCanEverTick = true;

        SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EsteEsNuestroRoot"));
        RootComponent = SceneComponent;

        PortalBoxA = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalBoxA"));
        PortalBoxB = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalBoxB"));

        PortalBoxA->SetupAttachment(RootComponent);
        PortalBoxB->SetupAttachment(RootComponent);

        // Hacer que generen overlaps
        PortalBoxA->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        PortalBoxA->SetCollisionResponseToAllChannels(ECR_Overlap);

        PortalBoxB->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        PortalBoxB->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
    Super::BeginPlay();

    PortalBoxA->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnPortalOverlapA);
    PortalBoxB->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnPortalOverlapB);
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
bool APortal::CanTeleport(AActor* Actor) const
{
    if (Cooldowns.Contains(Actor))
    {
        float Current = GetWorld()->GetTimeSeconds();
        return Current > Cooldowns[Actor];
    }
    return true;
}

void APortal::PutInCooldown(AActor* Actor)
{
    if (!Actor) return;

    float Current = GetWorld()->GetTimeSeconds();
    Cooldowns.Add(Actor, Current + CooldownTime);
}
void APortal::OnPortalOverlapA(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;
    if (!CanTeleport(OtherActor)) return;

    OtherActor->SetActorLocation(PortalBoxB->GetComponentLocation());
    PutInCooldown(OtherActor);
}
void APortal::OnPortalOverlapB(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;
    if (!CanTeleport(OtherActor)) return;

    OtherActor->SetActorLocation(PortalBoxA->GetComponentLocation());
    PutInCooldown(OtherActor);
}

