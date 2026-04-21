
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "WaterStream.generated.h"

UCLASS()
class CASITA_API AWaterStream : public AActor
{
    GENERATED_BODY()

public:
    AWaterStream();

    // Llamado por AValve cuando se cierra el agua
    void DeactivateStream();

private:
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* BlockingCollision;

    UPROPERTY(VisibleAnywhere)
    class UNiagaraComponent* WaterEffect;
};



