// Fill out your copyright notice in the Description page of Project Settings.


#include "Camara/NextLevel.h"
#include "Components/BoxComponent.h"
#include "Player/MainPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ANextLevel::ANextLevel()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    TriggerBox->SetGenerateOverlapEvents(true);
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ANextLevel::BeginPlay()
{
    Super::BeginPlay();
    
    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ANextLevel::OnOverlapBegin);
    }
    CameraManagerRef = Cast<ACameraManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraManager::StaticClass()));
}

void ANextLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OverlappedComponent || !OtherActor)
        return;

    if(OtherActor->IsA(AMainPlayer::StaticClass()) || OtherActor->IsA(ACharacter::StaticClass()))
    {
        
    }
    else
    {
        return;
	}
    // Control de disparo �nico
    if (bTriggerOnce && bAlreadyTriggered) return;
    bAlreadyTriggered = true;

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    if (bUseSplineCamera && CameraManagerRef)
    {
        CameraManagerRef->MoveToNextPoint();
        // El CameraManager se encarga del blend internamente
    }
    else if (!bUseSplineCamera && TargetCamera)
    {
        PC->SetViewTargetWithBlend(TargetCamera, BlendTime);
    }

    if (bTeleportPlayer && !PlayerNewLocation.IsZero())
    {
        OtherActor->SetActorLocation(PlayerNewLocation, false, nullptr,
            ETeleportType::TeleportPhysics);
    }
    if (bTriggerOnce)
    {
        Destroy();
        SetActorEnableCollision(false);
    }
}