#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GameWireframe.h"
#include "TheDotGamePlayerController.generated.h"

UCLASS()
class THEDOTGAME_API ATheDotGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void StartGame();

protected:
	void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameWireframe> GameWireframeClass;
	
private:
	FTimerHandle GameStateTimerHandle;
	UGameWireframe* GameWireframe = nullptr;
};
