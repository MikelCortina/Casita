#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InGameUI.generated.h"

UCLASS()
class CASITA_API UInGameUI : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ActualizarVisibilidadImagenes(int32 ImpulsosRestantes);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image1;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image2;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image3;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image4;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image5;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image6;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image7;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image8;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image9;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* Image10;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* TextBlock_133;
};