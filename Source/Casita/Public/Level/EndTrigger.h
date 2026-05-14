#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "EndTrigger.generated.h"

UCLASS()
class CASITA_API AEndTrigger : public AActor
{
    GENERATED_BODY()
public:
    AEndTrigger();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

public:
    UPROPERTY(EditAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> EndScreenWidgetClass;

private:
    bool bTriggered = false;
};