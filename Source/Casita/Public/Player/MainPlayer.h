#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

class UParticulasComponent;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class CASITA_API AMainPlayer : public APawn
{
    GENERATED_BODY()

public:
    AMainPlayer();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Collision")
    UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UParticulasComponent* ParticulasComponent;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 600.0f;

    float InputForward = 0.0f;
    float InputRight = 0.0f;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void ActivateParticles();
};