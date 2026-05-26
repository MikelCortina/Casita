#include "Components/AbsorptionComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

DEFINE_LOG_CATEGORY(LogAbsorptionComponent);

UAbsorptionComponent::UAbsorptionComponent()
{
    PrimaryComponentTick.bCanEverTick = false; 
}

void UAbsorptionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AbsorptionFXAsset)
    {
        FXComponent = NewObject<UNiagaraComponent>(this, TEXT("AbsorptionFX_Runtime"));
        FXComponent->RegisterComponent();
        FXComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        FXComponent->SetAsset(AbsorptionFXAsset);
        FXComponent->bAutoActivate = false; 
    }
}

void UAbsorptionComponent::StartEffect()
{
    if (FXComponent)
    {
        FXComponent->Activate(true);
    }
    bIsSucking = true;
}

void UAbsorptionComponent::StopEffect()
{
    if (FXComponent)
    {
        FXComponent->Deactivate();
    }
    bIsSucking = false;
}