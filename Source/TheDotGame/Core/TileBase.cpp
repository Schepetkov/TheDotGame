#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/Border.h>

#include "TileBase.h"
#include "GameWireframe.h"

UImage* UTileBase::GetTileBackground_Implementation()
{
	return nullptr;
}

UButton* UTileBase::GetTileButton_Implementation()
{
	return nullptr;
}

UBorder* UTileBase::GetTileBorder_Implementation()
{
	return nullptr;
}

UWidgetSwitcher* UTileBase::GetDotSwitcher_Implementation()
{
	return nullptr;
}

void UTileBase::SetWidgetOwner(UGameWireframe* InGameWireframe)
{
	GameWireframe = InGameWireframe;
}

void UTileBase::SetDot(EDotColor InDot)
{
	CurrentDot = InDot;
}

void UTileBase::MakeTileScore()
{
	bScoreTile = true;
	
	UImage* TileBackground = GetTileBackground();
	if (TileBackground)
	{
		TileBackground->SetVisibility(ESlateVisibility::Hidden);
	}
}

const EDotColor UTileBase::GetCurrentDot()
{
	return CurrentDot;
}

void UTileBase::NativeConstruct()
{
	Super::NativeConstruct();

	DotSwitcher = GetDotSwitcher();
	TileButton = GetTileButton();
	TileBorder = GetTileBorder();

	if (TileButton && TileBorder)
	{
		TileButton->OnHovered.AddDynamic(this, &UTileBase::OnHoveredTileButton);
		TileButton->OnUnhovered.AddDynamic(this, &UTileBase::OnUnhoveredTileButton);
		TileButton->OnClicked.AddDynamic(this, &UTileBase::OnClickedTileButton);
	}

}

void UTileBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (DotSwitcher)
	{
		switch (CurrentDot)
		{
		case EDotColor::None:
			DotSwitcher->SetActiveWidgetIndex(0);
			break;
		case EDotColor::Red:
			DotSwitcher->SetActiveWidgetIndex(1);
			break;
		case EDotColor::Green:
			DotSwitcher->SetActiveWidgetIndex(2);
			break;
		default:
			break;
		}	
	}
}

void UTileBase::OnHoveredTileButton()
{
	if (GameWireframe)
	{
		GameWireframe->SetHoveredTile(this);
	}
	
	if (TileBorder && !bScoreTile)
	{
		TileBorder->SetVisibility(ESlateVisibility::Visible);
	} 
}

void UTileBase::OnUnhoveredTileButton()
{
	if (TileBorder && !bScoreTile)
	{
		TileBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTileBase::OnClickedTileButton()
{
	if (GameWireframe && !bScoreTile)
	{
		GameWireframe->OnMouseButtonPressed(EDotColor::Green);
	}
}
