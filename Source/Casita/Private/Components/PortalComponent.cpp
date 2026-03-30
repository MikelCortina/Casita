#include "Components/PortalComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

UPortalComponent::UPortalComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Solo declaramos los punteros, los inicializamos en BeginPlay
    MainPortalComponent = nullptr;
    AbsorptionFXComponent = nullptr;
}

void UPortalComponent::BeginPlay()
{
    Super::BeginPlay();

    // --- 1. Portal Principal (Queremos que aparezca SIEMPRE) ---
    if (MainPortalAsset)
    {
        MainPortalComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
            MainPortalAsset,
            this,                                     // Componente al que se engancha
            NAME_None,                                // Nombre del socket (ninguno)
            FVector::ZeroVector,                      // Posición relativa
            FRotator(90.f, 90.f, 90.f),               // Rotación relativa
            EAttachLocation::SnapToTarget,            // Regla de adjuntado
            false,                                    // bAutoDestroy (No queremos que se destruya solo)
            true                                      // bAutoActivate (SÍ se activa al nacer)
        );
    }

    // --- 2. Absorción (Queremos que aparezca SOLO al moverse) ---
    if (AbsorptionAsset)
    {
        AbsorptionFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
            AbsorptionAsset,
            this,
            NAME_None,
            FVector::ZeroVector,
            FRotator(90.f, 90.f, 90.f),
            EAttachLocation::SnapToTarget,
            false,                                    // bAutoDestroy
            false                                     // bAutoActivate (¡FALSO! Empieza apagado de verdad)
        );

        // Por si las moscas, un seguro de vida extra
        if (AbsorptionFXComponent)
        {
            AbsorptionFXComponent->DeactivateImmediate();
        }
    }
}
void UPortalComponent::ActivatePortal()
{
    if (MainPortalComponent)
        MainPortalComponent->Activate(true);
}

void UPortalComponent::DeactivatePortal()
{
    if (MainPortalComponent)
        MainPortalComponent->Deactivate();
}

void UPortalComponent::ActivateAbsorption()
{
    if (AbsorptionFXComponent)
        AbsorptionFXComponent->Activate(true);
}

void UPortalComponent::DeactivateAbsorption()
{
    if (AbsorptionFXComponent)
        AbsorptionFXComponent->Deactivate();
}

void UPortalComponent::DestroyFX()
{
    if (MainPortalComponent)
    {
        MainPortalComponent->DeactivateImmediate();
        MainPortalComponent->DestroyComponent();
        MainPortalComponent = nullptr;
    }
    if (AbsorptionFXComponent)
    {
        AbsorptionFXComponent->DeactivateImmediate();
        AbsorptionFXComponent->DestroyComponent();
        AbsorptionFXComponent = nullptr;
    }
}