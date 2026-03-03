#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

class UParticulasComponent;

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

	// Función para llamar desde fuera (flores)
	void AnclarEnFlor(FVector FlorPos);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticulasComponent* ParticulasComponent;

	// Configuración de Crecimiento Orgánico
	UPROPERTY(EditAnywhere, Category = "Organic")
	float GrowSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Organic")
	float SmoothFactor = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Organic")
	float Vibracion = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Organic")
	float CaidaFactor = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Organic")
	float LateralFactor = 1.0f;

	// Estado interno
	FVector GrowDirection;
	FVector LastAnchorPosition;
	bool bTieneUltimaAncla = false;

	float InputForward = 0.0f;
	float InputRight = 0.0f;

	float distFromLastPoint = 0.0f;
	float stepSize = 10.0f; // Equivalente al stepSize de Unity (ajustado a escala UE)
	float tipRadius = 10.0f;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void ActivateParticles();

	UPROPERTY(EditAnywhere, Category = "Organic")
	float StickDistance = 150.0f; // Radio de búsqueda de paredes

	FVector CurrentSurfaceNormal = FVector::UpVector;
};