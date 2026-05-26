#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "ParticulasComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CASITA_API UParticulasComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UParticulasComponent();

    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraSystem* ParticleSystem;

    UPROPERTY(EditAnywhere, Category = "Effects")
    float Cooldown = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Effects")
    int32 MaxUses = 10;

    void ResetUses();
    void SpawnParticles();

protected:
    virtual void BeginPlay() override;

private:
    float LastSpawnTime  = -100.f;
    int32 UsesRemaining  = 0;
};