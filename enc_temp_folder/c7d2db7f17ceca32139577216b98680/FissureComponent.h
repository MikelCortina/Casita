// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "FissureComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CASITA_API UFissureComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFissureComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    // Llama a esto desde fuera si tu compañero necesita disparar el efecto manualmente
    UFUNCTION(BlueprintCallable, Category = "Fissure|VFX")
    void SpawnParticleExplosion();

private:
    // Asigna aquí tu Niagara System desde el editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fissure|VFX",
        meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNiagaraSystem> ExplosionEffect;

    // Escala visual del efecto
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fissure|VFX",
        meta = (AllowPrivateAccess = "true"))
    float EffectScale = 1.0f;

    // Segundos de espera entre activaciones
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fissure|Settings",
        meta = (AllowPrivateAccess = "true"))
    float TriggerCooldown = 1.0f;

    // Referencia al trigger del owner (se busca automáticamente en BeginPlay)
    UPROPERTY()
    TObjectPtr<class UBoxComponent> TriggerBox;

    bool bOnCooldown = false;
    FTimerHandle CooldownTimerHandle;

    UFUNCTION()
    void OnTriggerBeginOverlap(UPrimitiveComponent* OverlapComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
    bool bHasBeenTriggered = false;

};