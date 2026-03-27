#include "Components/PortalComponent.h"
#include "NiagaraComponent.h" 

UPortalComponent::UPortalComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    MainPortalComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MainPortalFX"));
    MainPortalComponent->SetupAttachment(this);

    MainPortalComponent->SetRelativeRotation(FRotator(90.f, 90.f, 90.f));

    AbsorptionComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AbsorptionFX"));
    AbsorptionComponent->SetupAttachment(this);

    AbsorptionComponent->bAutoActivate = false;
}

void UPortalComponent::BeginPlay()
{
    Super::BeginPlay();

    MainPortalComponent = NewObject<UNiagaraComponent>(this, TEXT("MainPortalFX"));
    if (MainPortalComponent)
    {
        MainPortalComponent->RegisterComponent();

        MainPortalComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

        // Rotación vertical DESPUÉS de atacharlo
        MainPortalComponent->SetRelativeRotation(FRotator(90.f, 90.f, 90.f));

        if (MainPortalAsset)
        {
            MainPortalComponent->SetAsset(MainPortalAsset);
            MainPortalComponent->Activate();
        }
    }

    // Repetir la misma lógica de AttachToComponent para el AbsorptionComponent...
}

void UPortalComponent::ActivateAbsorption()
{
 
    if (AbsorptionComponent)
    {
        AbsorptionComponent->Activate();
    }
}

void UPortalComponent::DeactivateAbsorption()
{
    // Función que ya llamas desde ANextLevel::CheckCameraMovement
    if (AbsorptionComponent)
    {
        AbsorptionComponent->Deactivate();
    }
}

void UPortalComponent::DestroyFX()
{
    // Destruimos las partículas de forma segura cuando el portal es de un solo uso
    if (MainPortalComponent)
    {
        MainPortalComponent->DestroyComponent();
    }

    if (AbsorptionComponent)
    {
        AbsorptionComponent->DestroyComponent();
    }
}