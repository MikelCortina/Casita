#include "Components/AbsorptionComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

UAbsorptionComponent::UAbsorptionComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // Por si quieres hacer lógica de succión en el Tick
}

void UAbsorptionComponent::BeginPlay()
{
    Super::BeginPlay();

    // Creamos el componente de Niagara en runtime para evitar errores de plantilla
    if (AbsorptionFXAsset)
    {
        FXComponent = NewObject<UNiagaraComponent>(this, TEXT("AbsorptionFX_Runtime"));
        FXComponent->RegisterComponent();
        FXComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        FXComponent->SetAsset(AbsorptionFXAsset);
        FXComponent->bAutoActivate = false; // Que empiece apagado
    }
}

void UAbsorptionComponent::StartEffect()
{
    if (FXComponent)
    {
        FXComponent->Activate(true);
    }
    bIsSucking = true;
    UE_LOG(LogTemp, Warning, TEXT("Efecto de absorción ACTIVADO"));
}

void UAbsorptionComponent::StopEffect()
{
    if (FXComponent)
    {
        FXComponent->Deactivate();
    }
    bIsSucking = false;
    UE_LOG(LogTemp, Warning, TEXT("Efecto de absorción DESACTIVADO"));
}