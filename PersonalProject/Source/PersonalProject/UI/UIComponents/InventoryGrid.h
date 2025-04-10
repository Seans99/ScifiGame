#pragma once

#include "CoreMinimal.h"
#include "../GUIBase.h"
#include "PersonalProject/Structs/ItemStruct.h"
#include "PersonalProject/Structs/Line.h"
#include "InventoryGrid.generated.h"

class UCustomDragAndDropOperation;

class UInventoryComponent;
class UItemSlotUI;
class AItemBase;

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
	virtual bool NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* GridBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* GridCanvasPanel;

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UItemSlotUI> ItemSlotWidgetClass;

	UPROPERTY()
	UItemSlotUI* ItemSlotWidget;

public:
	void InitializeGrid(UInventoryComponent* InventoryComponent, float TileSize);

public:
	UFUNCTION()
	void Remove(FItemData& Item);

	UFUNCTION()
	void Refresh();

private:
	void CreateLineSegments();
	FItemData* GetPayLoad(UCustomDragAndDropOperation* DragDropOperation);
	bool CheckIfRoomAvailable(FItemData* Payload);
	bool MousePosInTile(FVector2D MousePos, bool& Right, bool& Down);
	
public:
	UPROPERTY()
	UInventoryComponent* InventoryComp;

	float TileSize;

private:
	TArray<FLine> Lines;
	FIntPoint DraggedItemTile;
};
