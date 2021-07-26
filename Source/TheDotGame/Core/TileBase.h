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
	
private:
	UGameWireframe* GameWireframe = nullptr;
	UBorder* TileBorder = nullptr;
	UWidgetSwitcher* DotSwitcher = nullptr;

	EDotColor CurrentDot = EDotColor::None;
	bool bScoreTile = false;
};
