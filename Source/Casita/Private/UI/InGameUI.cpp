#include "UI/InGameUI.h"

void UInGameUI::ActualizarVisibilidadImagenes(int32 ImpulsosRestantes)
{
    TArray<UImage*> Imagenes = {
        Image1, Image2, Image3, Image4, Image5,
        Image6, Image7, Image8, Image9, Image10
    };

    for (int32 i = 0; i < Imagenes.Num(); i++)
    {
        if (Imagenes[i])
        {
            ESlateVisibility Vis = (ImpulsosRestantes >= i + 1)
                ? ESlateVisibility::Visible
                : ESlateVisibility::Hidden;
            Imagenes[i]->SetVisibility(Vis);
        }
    }
}