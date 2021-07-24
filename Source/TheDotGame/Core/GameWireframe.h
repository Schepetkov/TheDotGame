#pragma once
#include "CoreMinimal.h"

#include "TileBase.h"
#include "GameWireframe.generated.h"

class UHorizontalBox;
class UWidgetSwitcher;
class UButton;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Intro,
	Game,
	GameOver
};

UCLASS()
class THEDOTGAME_API UGameWireframe : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHoveredTile(UTileBase* InHoveredTile);
	void OnMouseButtonPressed(EDotColor InDotColor);
	
	UFUNCTION(BlueprintCallable)
	void SwitchGameState(EGameState InGameState);
	
protected:
	//Catch right mouse button click
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativePreConstruct() override;
	
	void CreateGrid();
	void AddScore(EDotColor InDotColor);
		
	UFUNCTION()
	void OnClickedStartGameButton();
	
	UFUNCTION()
	void RestartGameWireframe();

	UFUNCTION()
	void PrepareToRestartGame();
	
	UFUNCTION()
	void ShowRandomDot();

	UFUNCTION(BlueprintNativeEvent)
	UWidgetSwitcher* GetGameStateWidgetSwitcher();
	virtual UWidgetSwitcher* GetGameStateWidgetSwitcher_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	UHorizontalBox* GetScoreRow();
	virtual UHorizontalBox* GetScoreRow_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	TArray<UHorizontalBox*> GetAllRows();
	virtual TArray<UHorizontalBox*> GetAllRows_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	UButton* GetStartGameButton();
	virtual UButton* GetStartGameButton_Implementation();

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 ScoreCounter {0};

	UPROPERTY(BlueprintReadOnly)
	int32 CountdownCounter {5};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UTileBase> TileBaseClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundBase* RedDotPickupSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundBase* GreenDotPickupSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundBase* GameOverLoseSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundBase* GameOverWinSound = nullptr;

private:
	TArray<UHorizontalBox*> AllRows;
	TArray<UTileBase*> AllScoreTiles;
	TArray<UTileBase*> AllGameTiles;

	UTileBase* HoveredTile = nullptr;

	FTimerHandle DotSpawnTimerHandle;
	FTimerHandle RestartGameTimerHandle;
	int32 CurrentRandomDotIndex {0};
};
