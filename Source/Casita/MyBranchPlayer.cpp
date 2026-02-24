// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBranchPlayer.h"

AMyBranchPlayer::MyBranchPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    BranchSpline = CreateDefaultSubobject<USplineComponent>(TEXT("BranchSpline"));
    RootComponent = BranchSpline;
}

AMyBranchPlayer::~MyBranchPlayer()
{
}

void AMyBranchPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyBranchPlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyBranchPlayer::MoveRight);
}

void AMyBranchPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (InputDirection.IsNearlyZero()) return;

    FVector CurrentLoc = GetActorLocation();
    FVector DesiredMove = InputDirection * MoveSpeed * DeltaTime;
    FVector NextLoc = CurrentLoc + DesiredMove;

    // Rayo hacia la pared (ajusta el eje según tu cámara frontal)
    FHitResult Hit;
    FVector TraceStart = NextLoc + FVector(0, 0, 500); // Rayo desde "afuera"
    FVector TraceEnd = NextLoc - FVector(0, 0, 500);

    if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility))
    {
        SetActorLocation(Hit.ImpactPoint);

        // ¿Es hora de añadir un nuevo trozo de rama?
        if (FVector::Dist(Hit.ImpactPoint, LastPointLocation) > SegmentLength)
        {
            AddBranchSegment(Hit.ImpactPoint);
        }
    }
}

void MyBranchPlayer::AddBranchSegment(FVector NewLocation)
{
    // 1. Añadir punto al Spline
    BranchSpline->AddSplinePoint(NewLocation, ESplineCoordinateSpace::World);
    int32 PointIndex = BranchSpline->GetNumberOfSplinePoints() - 1;

    if (PointIndex > 0)
    {
        // 2. Crear el componente de malla dinámicamente
        USplineMeshComponent* MeshSegment = NewObject<USplineMeshComponent>(this);
        MeshSegment->SetStaticMesh(BranchMesh);
        MeshSegment->SetForwardAxis(ESplineMeshAxis::X); // Asegúrate que tu mesh mire a X
        MeshSegment->RegisterComponent();
        MeshSegment->AttachToComponent(BranchSpline, FAttachmentTransformRules::KeepWorldTransform);

        // 3. Obtener posiciones y tangentes para curvar la malla
        FVector StartPos, StartTangent, EndPos, EndTangent;
        BranchSpline->GetLocationAndTangentAtSplinePoint(PointIndex - 1, StartPos, StartTangent, ESplineCoordinateSpace::Local);
        BranchSpline->GetLocationAndTangentAtSplinePoint(PointIndex, EndPos, EndTangent, ESplineCoordinateSpace::Local);

        // 4. Aplicar la deformación
        MeshSegment->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);

        // Colisión opcional
        MeshSegment->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    LastPointLocation = NewLocation;
}