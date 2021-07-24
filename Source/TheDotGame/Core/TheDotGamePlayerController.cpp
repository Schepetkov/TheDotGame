#include "TheDotGamePlayerController.h"

void ATheDotGamePlayerController::StartGame()
{
	if (GameWireframe)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameStateTimerHandle);
		GameWireframe->SwitchGameState(EGameState::Game);
	}
}

void ATheDotGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(GameWireframeClass);

	GameWireframe = CreateWidget<UGameWireframe>(this, GameWireframeClass);

	if(GameWireframe)
	{
		GameWireframe->AddToViewport();
		GameWireframe->SwitchGameState(EGameState::Intro);
		GetWorldTimerManager().SetTimer(GameStateTimerHandle, this, &ATheDotGamePlayerController::StartGame, 5.f, false);
	}
};
