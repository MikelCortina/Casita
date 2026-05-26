// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterStream/HardWaterStream.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"


AHardWaterStream::AHardWaterStream()
{
    PrimaryActorTick.bCanEverTick = false;

    BlockingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingCollision"));
    RootComponent = BlockingCollision;
    BlockingCollision->SetCollisionProfileName(TEXT("BlockAll"));

    WaterEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WaterEffect"));
    WaterEffect->SetupAttachment(RootComponent);
}

void AHardWaterStream::DeactivateStream()
{
    BlockingCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (WaterEffect)
        WaterEffect->Deactivate();

}

