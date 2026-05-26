// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterStream/HardWaterStream.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AHardWaterStream::AHardWaterStream()
{
    PrimaryActorTick.bCanEverTick = false;

    BlockingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingCollision"));
    RootComponent = BlockingCollision;
    // Bloquea al jugador pero no a otros objetos
    BlockingCollision->SetCollisionProfileName(TEXT("BlockAll"));

    WaterEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WaterEffect"));
    WaterEffect->SetupAttachment(RootComponent);
}

void AHardWaterStream::DeactivateStream()
{
    // Desactiva la colisión para que el jugador pueda pasar
    BlockingCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Apaga el Niagara
    if (WaterEffect)
        WaterEffect->Deactivate();

    UE_LOG(LogTemp, Warning, TEXT("Corriente de agua desactivada"));
}

