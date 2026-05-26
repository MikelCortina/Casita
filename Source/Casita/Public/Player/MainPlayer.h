#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UI/InGameUI.h"
#include "MainPlayer.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticulasComponent;
class UInstancedStaticMeshComponent;
class AValve; 

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticulasComponent* ParticulasComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Branch Trail")
    UInstancedStaticMeshComponent* InstancedTrailComponent;

    UFUNCTION(BlueprintImplementableEvent)
    void UpdateUI();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Trail")
    float SpawnTrailDistance = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Trail")
    FVector TrailScale = FVector(0.5f, 0.5f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GravityFactor = 0.5f;

    void SetCameraIsMoving(bool bNewValue)
    {
        bCameraMoving = bNewValue;
        SetActorHiddenInGame(bNewValue);
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    int32 MaxCheckpointUses = 10;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
    int32 RemainingCheckpoints = 10;

    void ResetPlayerState();
    void SetCheckpoint();
    void ResetGravity();

    UPROPERTY()
    AValve* NearbyValve;

    void SetNearbyValve(AValve* Valve) { NearbyValve = Valve; }
    void ClearNearbyValve() { NearbyValve = nullptr; }
    void TryInteractWithValve();

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> InGameUIClass;

private:
    UUserWidget* PauseMenuInstance = nullptr;
    bool bIsPaused = false;
    void TogglePause();

    float InputForward = 0.0f;
    float InputRight = 0.0f;
    FVector LastCheckpointLocation;
    float MaxDistanceReached;
    bool bHasCheckpoint = false;
    bool bCameraMoving = false;
    FVector LastTrailLocation;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void ActivateParticles();

    UInGameUI* InGameUIInstance = nullptr;
};