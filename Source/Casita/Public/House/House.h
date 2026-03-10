#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "House.generated.h"

UCLASS()
class CASITA_API AHouse : public AActor
{
	GENERATED_BODY()

public:
	AHouse();

protected:
	virtual void BeginPlay() override;
};