#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TileBase.generated.h"

class UImage;
class UWidgetSwitcher;
class UButton;
class UBorder;
class UGameWireframe;

UENUM(BlueprintType)
enum class EDotColor : uint8
{
	None,
	Red,
	Green
};

UCLASS()
class THEDOTGAME_API UTileBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidgetOwner(UGameWireframe* InGameWireframe);
	void SetDot(EDotColor InDot);
	const EDotColor GetCurrentDot();
	void MakeTileScore();

protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnHoveredTileButton();
	
	UFUNCTION()
	void OnUnhoveredTileButton();

	//Catch left mouse button click
	UFUNCTION()
	void OnClickedTileButton();
	
	UFUNCTION(BlueprintNativeEvent)
	UImage* GetTileBackground();
	virtual UImage* GetTileBackground_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	UButton* GetTileButton();
	virtual UButton* GetTileButton_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	UBorder* GetTileBorder();
	virtual UBorder* GetTileBorder_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	UWidgetSwitcher* GetDotSwitcher();
	virtual UWidgetSwitcher* GetDotSwitcher_Implementation();

private:
	UGameWireframe* GameWireframe = nullptr;
	UButton* TileButton = nullptr;
	UBorder* TileBorder = nullptr;
	UWidgetSwitcher* DotSwitcher = nullptr;

	EDotColor CurrentDot = EDotColor::None;

	bool bScoreTile = false;
};
