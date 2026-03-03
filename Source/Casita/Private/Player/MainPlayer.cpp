#include "Player/MainPlayer.h"
#include "Components/ParticulasComponent.h"
#include "Components/InputComponent.h"

AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	ParticulasComponent = CreateDefaultSubobject<UParticulasComponent>(TEXT("ParticulasComponent"));

	GrowDirection = FVector::UpVector;
	LastAnchorPosition = FVector::ZeroVector;
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. Declarar vectores de referencia con valores por defecto (Evita el error C2065)
    FVector WallUp = FVector::UpVector;
    FVector WallRight = FVector::RightVector;
    FVector TargetDir = FVector::ZeroVector;

    // 2. BUSCAR PARED (Raycast)
    FHitResult SurfHit;
    FVector TraceStart = GetActorLocation();
    FVector TraceEnd = TraceStart + (FVector::ForwardVector * -StickDistance);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bOnWall = GetWorld()->LineTraceSingleByChannel(SurfHit, TraceStart, TraceEnd, ECC_Visibility, Params);

    if (bOnWall)
    {
        CurrentSurfaceNormal = SurfHit.Normal;

        // Calculamos los ejes basados en la pared
        WallUp = FVector::VectorPlaneProject(FVector::UpVector, CurrentSurfaceNormal).GetSafeNormal();

        // Si la pared es un suelo o techo (normal paralela al UpVector), el proyecto daría cero.
        // En ese caso, usamos el Forward del mundo como "Arriba" temporal.
        if (WallUp.IsNearlyZero()) { WallUp = FVector::ForwardVector; }

        WallRight = FVector::CrossProduct(WallUp, CurrentSurfaceNormal).GetSafeNormal();

        // Aplicamos el Input del usuario a los ejes de la PARED
        TargetDir = (WallUp * InputForward) + (WallRight * InputRight);
    }
    else
    {
        // Movimiento libre en el aire si no hay pared
        TargetDir = FVector(0.f, InputRight, InputForward);
    }

    if (!TargetDir.IsNearlyZero() || bTieneUltimaAncla)
    {
        TargetDir.Normalize();

        // 3. LÓGICA ORGÁNICA (Vibración y Curvatura)
        if (bTieneUltimaAncla)
        {
            FVector ToAnchor = LastAnchorPosition - GetActorLocation();

            // Corregido: Ahora WallRight existe aquí y el DotProduct devuelve el float que Sign necesita
            float SideDot = FVector::DotProduct(ToAnchor, WallRight);
            TargetDir += (WallRight * -FMath::Sign(SideDot) * LateralFactor);

            TargetDir.Z -= ToAnchor.Size() * (CaidaFactor / 100.0f);
        }

        // Añadimos un poco de ruido visual
        TargetDir.Y += FMath::RandRange(-Vibracion, Vibracion) * 0.01f;
        TargetDir.Z += FMath::RandRange(-Vibracion, Vibracion) * 0.01f;

        // 4. INTERPOLACIÓN Y MOVIMIENTO
        GrowDirection = FMath::VInterpTo(GrowDirection, TargetDir.GetSafeNormal(), DeltaTime, SmoothFactor);

        float MoveDist = GrowSpeed * DeltaTime;
        FVector NextPos = GetActorLocation() + (GrowDirection * MoveDist);

        // 5. AJUSTE DE "IMÁN" A LA PARED
        if (bOnWall)
        {
            float TargetX = SurfHit.ImpactPoint.X + (CurrentSurfaceNormal.X * 20.0f);
            NextPos.X = FMath::FInterpTo(NextPos.X, TargetX, DeltaTime, 10.0f);
        }

        SetActorLocation(NextPos, true);
    }
}void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SpawnParticles", IE_Pressed, this, &AMainPlayer::ActivateParticles);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
}

void AMainPlayer::MoveForward(float Value) { InputForward = Value; }
void AMainPlayer::MoveRight(float Value) { InputRight = Value; }

void AMainPlayer::AnclarEnFlor(FVector FlorPos)
{
	LastAnchorPosition = FlorPos;
	bTieneUltimaAncla = true;
}

void AMainPlayer::ActivateParticles()
{
	if (ParticulasComponent)
	{
		ParticulasComponent->SpawnParticles();
	}
}