#pragma once

#include "CoreMinimal.h"
#include "../GUIBase.h"
#include "PersonalProject/Structs/Line.h"
#include "InventoryGrid.generated.h"

class UInventoryComponent;

class UCanvasPanel;
class UBorder;


UCLASS()
class PERSONALPROJECT_API UInventoryGrid : public UGUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
		) const override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* GridBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* GridCanvasPanel;

public:
	void InitializeGrid(UInventoryComponent* InventoryComponent, float TileSize);

private:
	void CreateLineSegments();
	
public:
	UPROPERTY()
	UInventoryComponent* InventoryComp;

	float TileSize;

private:
	TArray<FLine> Lines;
};
