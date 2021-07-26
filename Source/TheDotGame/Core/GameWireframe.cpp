#include <Blueprint/UserWidget.h>
#include <Blueprint/WidgetTree.h>
#include <Components/HorizontalBox.h>
#include <Components/WidgetSwitcher.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

#include "GameWireframe.h"
#include "TheDotGamePlayerController.h"

void UGameWireframe::ShowRandomDot()
{
	if (AllGameTiles.Num() > 0)
	{
		if (AllGameTiles.IsValidIndex(CurrentRandomDotIndex))
		{
			AllGameTiles[CurrentRandomDotIndex]->SetDot(EDotColor::None);
		}
		
		CurrentRandomDotIndex = UKismetMathLibrary::RandomIntegerInRange(0, AllGameTiles.Num());

		if (AllGameTiles.IsValidIndex(CurrentRandomDotIndex))
		{
			AllGameTiles[CurrentRandomDotIndex]->SetDot(EDotColor(UKismetMathLibrary::RandomIntegerInRange(1, 2)));
		}
	}
}

void UGameWireframe::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (WidgetTree)
	{
		if (UButton* StartGameButton = Cast<UButton>(WidgetTree->FindWidget("StartGameButton")))
		{
			if (StartGameButton)
			{
				StartGameButton->OnClicked.AddDynamic(this, &UGameWireframe::OnClickedStartGameButton);
			}
		}
	}
	
	CreateGrid();
}

void UGameWireframe::OnMouseButtonPressed(EDotColor InDotColor)
{
	if (AllGameTiles.Num() > 0)
	{
		if (AllGameTiles.IsValidIndex(CurrentRandomDotIndex) && HoveredTile)
		{
			UTileBase* CurrentTile = AllGameTiles[CurrentRandomDotIndex];

			if (CurrentTile)
			{
				if (CurrentTile != HoveredTile)
				{
					return;
				}
				
				if (InDotColor == CurrentTile->GetCurrentDot())
				{
					AddScore(InDotColor);
					CurrentTile->SetDot(EDotColor::None);
				}
				else
				{
					SwitchGameState(EGameState::GameOver);
					
					if (GameOverLoseSound)
					{
						UGameplayStatics::PlaySound2D(this, GameOverLoseSound);
					}
				}
			}
		}
	}
}

void UGameWireframe::SwitchGameState(EGameState InGameState)
{
	if (WidgetTree)
	{
		if (UWidgetSwitcher* GameStateWidgetSwitcher = Cast<UWidgetSwitcher>(WidgetTree->FindWidget("GameStateWidgetSwitcher")))
		{
			if (GameStateWidgetSwitcher)
			{
				switch (InGameState)
				{
				case EGameState::Intro:
					GameStateWidgetSwitcher->SetActiveWidgetIndex(0);
					break;
				case EGameState::Game:
					GameStateWidgetSwitcher->SetActiveWidgetIndex(1);

					GetWorld()->GetTimerManager().ClearTimer(RestartGameTimerHandle);
					GetWorld()->GetTimerManager().SetTimer(DotSpawnTimerHandle, this, &UGameWireframe::ShowRandomDot, 2.f, true);

					break;
				case EGameState::GameOver:
					GameStateWidgetSwitcher->SetActiveWidgetIndex(2);

					GetWorld()->GetTimerManager().ClearTimer(DotSpawnTimerHandle);
					GetWorld()->GetTimerManager().SetTimer(RestartGameTimerHandle, this, &UGameWireframe::PrepareToRestartGame, 1.f, true);

					break;
				default:
					break;
				}
			}
		}
	}
}

void UGameWireframe::SetHoveredTile(UTileBase* InHoveredTile)
{
	HoveredTile = InHoveredTile;
}

TArray<UHorizontalBox*> UGameWireframe::GetAllRows_Implementation()
{
	TArray<UHorizontalBox*> GridRows;
	
	return GridRows;
}

void UGameWireframe::CreateGrid()
{
	check(TileBaseClass);
	
	AllRows = GetAllRows();

	int32 RowCounter {0};
	while (RowCounter != 10)
	{
		for (int32 i = 0; i < 10; i++)
		{
			if (AllRows.Num() > 0 && AllRows.IsValidIndex(RowCounter))
			{
				if (UTileBase* GameTile = CreateWidget<UTileBase>(this, TileBaseClass))
				{
					AllRows[RowCounter]->AddChild(GameTile);
					GameTile->SetWidgetOwner(this);
					AllGameTiles.Add(GameTile);
				}
			}
		}
		
		RowCounter++;
	}
	
	if (WidgetTree)
	{
		if (UHorizontalBox* ScoreRow = Cast<UHorizontalBox>(WidgetTree->FindWidget("ScoreHorizontalBoxRow")))
		{
			if (ScoreRow)
			{
				for (int32 i = 0; i < 10; i++)
				{
					if (UTileBase* ScoreTile = Cast<UTileBase>(CreateWidget<UTileBase>(this, TileBaseClass)))
					{
						ScoreRow->AddChild(ScoreTile);
						ScoreTile->MakeTileScore();
						AllScoreTiles.Add(ScoreTile);
					}
				}
			}
		}
	}
}

void UGameWireframe::AddScore(EDotColor InDotColor)
{
	if (AllScoreTiles.Num() > 0 && AllScoreTiles.IsValidIndex(ScoreCounter))
	{
		AllScoreTiles[ScoreCounter]->SetDot(InDotColor);
		
		if (InDotColor == EDotColor::Red)
		{
			if (RedDotPickupSound)
			{
				UGameplayStatics::PlaySound2D(this, RedDotPickupSound);
			}
		}
		else
		{
			if (GreenDotPickupSound)
			{
				UGameplayStatics::PlaySound2D(this, GreenDotPickupSound);
			}
		}
		
		ScoreCounter++;

		if (ScoreCounter == 10)
		{
			SwitchGameState(EGameState::GameOver);
			
			if (GameOverWinSound)
			{
				UGameplayStatics::PlaySound2D(this, GameOverWinSound);
			}
		}
	}
}

void UGameWireframe::OnClickedStartGameButton()
{
	if (GetWorld())
	{
		if (ATheDotGamePlayerController* LocalController = Cast<ATheDotGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			LocalController->StartGame();
		}
	}
}

void UGameWireframe::RestartGameWireframe()
{
	for (auto ScoreTile : AllScoreTiles)
	{
		ScoreTile->SetDot(EDotColor::None);
	}
	
	SwitchGameState(EGameState::Game);
	
	ScoreCounter = 0;
	CountdownCounter = 5;
}

void UGameWireframe::PrepareToRestartGame()
{
	if (CountdownCounter == 0)
	{
		RestartGameWireframe();
		return;
	}
	
	CountdownCounter--;
}

FReply UGameWireframe::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown("RightMouseButton"))
	{
		OnMouseButtonPressed(EDotColor::Red);
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
