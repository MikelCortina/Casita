#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterStream.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UNiagaraComponent;

UCLASS()
class CASITA_API AWaterStream : public AActor
{
    GENERATED_BODY()

public:
    AWaterStream();

protected:
    virtual void BeginPlay() override;

public:
    // Componente que define el área del chorro de agua
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerVolume;

    // Componente visual (para que le pongas un material de agua semitransparente o partículas)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WaterVisualMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UNiagaraComponent* LowWaterParticles;

    // Cuánto queremos multiplicar la gravedad mientras esté dentro
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Stream")
    float GravityMultiplier = 2.0f;

    // Función que se llama cuando el jugador ENTRA al chorro
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Función que se llama cuando el jugador SALE del chorro
    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};