#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/Border.h>
#include <Blueprint/WidgetTree.h>

#include "TileBase.h"
#include "GameWireframe.h"

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

	if (UImage* TileBackground = Cast<UImage>(WidgetTree->FindWidget("TileBackground")))
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

	if (WidgetTree)
	{
		TileBorder = Cast<UBorder>(WidgetTree->FindWidget("TileBorder"));
		DotSwitcher = Cast<UWidgetSwitcher>(WidgetTree->FindWidget("DotWidgetSwitcher"));

		if (UButton* TileButton = Cast<UButton>(WidgetTree->FindWidget("TileButton")))
		{
			if (TileButton)
			{
				TileButton->OnHovered.AddDynamic(this, &UTileBase::OnHoveredTileButton);
				TileButton->OnUnhovered.AddDynamic(this, &UTileBase::OnUnhoveredTileButton);
				TileButton->OnClicked.AddDynamic(this, &UTileBase::OnClickedTileButton);
			}
		}
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
