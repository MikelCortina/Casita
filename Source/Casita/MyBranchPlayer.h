// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "MyBranchPlayer.generated.h"

/**
 * 
 */
class CASITA_API MyBranchPlayer : public APawn
{
public:
	MyBranchPlayer();
	~MyBranchPlayer();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Componentes
    UPROPERTY(VisibleAnywhere)
    USplineComponent* BranchSpline;

    UPROPERTY(EditAnywhere, Category = "Branch")
    UStaticMesh* BranchMesh; // El cilindro/rama

    // Configuración
    UPROPERTY(EditAnywhere)
    float SegmentLength = 50.f;

    UPROPERTY(EditAnywhere)
    float MoveSpeed = 300.f;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void AddBranchSegment(FVector NewLocation);

    FVector InputDirection;
    FVector LastPointLocation;
};
