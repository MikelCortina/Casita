// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "HardWaterStream.generated.h"

UCLASS()
class CASITA_API AHardWaterStream : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHardWaterStream();

    // Llamado por AValve cuando se cierra el agua
    void DeactivateStream();

private:
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* BlockingCollision;

    UPROPERTY(VisibleAnywhere)
    class UNiagaraComponent* WaterEffect;

};
