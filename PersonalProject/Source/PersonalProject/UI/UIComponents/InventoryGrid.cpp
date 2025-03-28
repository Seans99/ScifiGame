#include "InventoryGrid.h"

#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "PersonalProject/Components/InventoryComponent.h"

void UInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

int32 UInventoryGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	for (auto Line : Lines)
	{
		TArray<FVector2D> LinePoints;

		FGeometry CachedGeom = GridBorder->GetCachedGeometry();
		LinePoints.Add(Line.Start + FVector2D(0, 0));
		LinePoints.Add(Line.End + FVector2D(0, 0));
		
		FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId + 1,
		AllottedGeometry.ToPaintGeometry(),
		LinePoints,
		ESlateDrawEffect::None,
		FLinearColor::White,  // Line Color
		true,  // Anti-aliasing
		2.0f   // Thickness
		);
	}

	// Draw drop location...
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}

void UInventoryGrid::InitializeGrid(UInventoryComponent* InventoryComponent, float NewTileSize)
{
	InventoryComp = InventoryComponent;
	TileSize = NewTileSize;
	
	UPanelSlot* GridBorderSlot = GridBorder->Slot;
	if (GridBorderSlot)
	{
		if (UCanvasPanelSlot* GridSlot = Cast<UCanvasPanelSlot>(GridBorderSlot))
		{
			FVector2D GridSize = FVector2D((InventoryComponent->Columns * NewTileSize) , (InventoryComponent->Rows * NewTileSize));
			GridSlot->SetSize(GridSize);
			CreateLineSegments();
		}
	}
}

void UInventoryGrid::CreateLineSegments()
{
	// Create vertical lines
	for (int i = InventoryComp->Columns; i >= 0; i--)
	{
		int localX = i * TileSize;
		FVector2D Start = FVector2D(localX, 0);
		FVector2D End = FVector2D(localX, (InventoryComp->Rows * TileSize));
		FLine Line(Start, End);
		
		Lines.Add(Line);
	}

	// Create horizontal lines
	for (int i = InventoryComp->Rows; i >= 0; i--)
	{
		int localY = i * TileSize;
		FVector2D Start = FVector2D(0, localY);
		FVector2D End = FVector2D((InventoryComp->Columns * TileSize), localY);
		FLine Line(Start, End);
		
		Lines.Add(Line);
	}
}
