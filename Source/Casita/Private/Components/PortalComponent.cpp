#include "Components/PortalComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"


UPortalComponent::UPortalComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    MainPortalComponent = nullptr;
    AbsorptionFXComponent = nullptr;
}

void UPortalComponent::BeginPlay()
{
    Super::BeginPlay();

    if (MainPortalAsset)
    {
        MainPortalComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
            MainPortalAsset,
            this,                                    
            NAME_None,                               
            FVector::ZeroVector,                      
            FRotator(90.f, 90.f, 90.f),               
            EAttachLocation::SnapToTarget,            
            false,                                    
            true                                      
        );
    }

    if (AbsorptionAsset)
    {
        AbsorptionFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
            AbsorptionAsset,
            this,
            NAME_None,
            FVector::ZeroVector,
            FRotator(90.f, 90.f, 90.f),
            EAttachLocation::SnapToTarget,
            false,                                    
            false                                     
        );

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