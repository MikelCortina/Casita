#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Valve.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UUserWidget;
class AMainPlayer;
class AHardWaterStream;

UCLASS()
class CASITA_API AValve : public AActor
{
    GENERATED_BODY()

public:
    AValve();
    void TryInteract();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valve")
    TObjectPtr<AHardWaterStream> LinkedWaterStream;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valve")
    float RotationSpeed = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valve")
    float SpinDuration = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> InteractPromptClass;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* ProximityTrigger;

    UPROPERTY(VisibleAnywhere)
    UUserWidget* InteractPromptWidget;

    UFUNCTION()
    void OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

private:
    bool bPlayerInRange = false;
    bool bActivated = false;
    bool bSpinning = false;
    float SpinTimer = 0.f;

    void ShowInteractPrompt();
    void HideInteractPrompt();
};