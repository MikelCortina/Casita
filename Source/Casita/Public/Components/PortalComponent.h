#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PortalComponent.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CASITA_API UPortalComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UPortalComponent();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal FX")
    UNiagaraSystem* MainPortalAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal FX")
    UNiagaraSystem* AbsorptionAsset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal FX")
    UNiagaraComponent* MainPortalComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal FX")
    UNiagaraComponent* AbsorptionFXComponent;

    void ActivatePortal();
    void DeactivatePortal();
    void ActivateAbsorption();
    void DeactivateAbsorption();
    void DestroyFX();
};