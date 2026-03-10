// Fill out your copyright notice in the Description page of Project Settings.


#include "Camara/NextLevel.h"
#include "Components/BoxComponent.h"
#include "Player/MainPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ANextLevel::ANextLevel()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName("Trigger");

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ANextLevel::OnOverlapBegin);
}

void ANextLevel::BeginPlay()
{
	Super::BeginPlay();
}

void ANextLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
	if (!Player) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && TargetCamera)
	{
		// Rotar la cámara hacia el lateral de la casa
		PC->SetViewTargetWithBlend(TargetCamera, BlendTime);
	}

	// Teletransportar al jugador a la nueva posición
	if (!PlayerNewLocation.IsZero())
	{
		Player->SetActorLocation(PlayerNewLocation);
	}
}