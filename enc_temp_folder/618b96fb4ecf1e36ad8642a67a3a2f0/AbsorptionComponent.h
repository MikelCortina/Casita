#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AbsorptionComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CASITA_API UAbsorptionComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UAbsorptionComponent();

protected:
    virtual void BeginPlay() override;

public:
    // El asset de Niagara (lo que asignas en el editor)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
    UNiagaraSystem* AbsorptionFXAsset;

    // El componente que reproduce el asset
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
    UNiagaraComponent* FXComponent;

    // Funciones de control
    void StartEffect();
    void StopEffect();

    // Si quieres que succione al jugador, podemos usar esta flag
    bool bIsSucking = false;
};