#include "InventoryGrid.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "PersonalProject/Components/InventoryComponent.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerCharacter.h"
#include "PersonalProject/PrimarySystems/GameInstances/DropAtPlayer.h"
#include "PersonalProject/UI/ItemSlotUI.h"

void UInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

// Draw the grid lines
int32 UInventoryGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	for (auto Line : Lines)
	{
		TArray<FVector2D> LinePoints;

		LinePoints.Add(Line.Start + FVector2D(0, 0));
		LinePoints.Add(Line.End + FVector2D(0, 0));

		FLinearColor CustomColor(0.033105f, 0.097587f, 0.130136f, 0.901961f);
		
		FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		LinePoints,
		ESlateDrawEffect::None,
		CustomColor,  // Line Color
		true,
		2.0f   // Thickness
		);
	}
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}

// When dropping item inside the grid widget checks if space and then adds the items with respective index to inventory array.
// if no space it will check if there is space anywhere else, otherwise it will drop the item in front of the player
bool UInventoryGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	UCustomDragAndDropOperation* Operation = Cast<UCustomDragAndDropOperation>(InOperation);
	
	if (CheckIfRoomAvailable(Operation->ItemData))
	{
		FTile Tile = FTile(DraggedItemTile.X, DraggedItemTile.Y);
		int Index = InventoryComp->TileToIndex(Tile);
		InventoryComp->AddItemToInventoryArray(Operation->ItemData, Index);
	}
	else
	{
		if (!InventoryComp->AddToInventory(Operation->ItemData))
		{
			if (UDropAtPlayer* DropAtPlayer = GetGameInstance()->GetSubsystem<UDropAtPlayer>())
			{
				DropAtPlayer->Drop(InOperation->Payload, &Operation->ItemData);
			}
		}
	}
	
	return true;
}

// Runs when dragging item over the grid widget
// Sets the DraggedItemTile which holds the tiles that will be occupied by the item
bool UInventoryGrid::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	
	UCustomDragAndDropOperation* Operation = Cast<UCustomDragAndDropOperation>(InOperation);

	// Get mouse position on widget.
	FVector2D MousePos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

	bool Right = false;
	bool Down = false;

	MousePosInTile(MousePos, Right, Down);
	FVector2D ItemDimensions = Operation->ItemData.GridDimensions;
	
	int ItemDimensionX;
	int ItemDimensionY;
	
	Right ? ItemDimensionX = ItemDimensions.X - 1 : ItemDimensionX = ItemDimensions.X;
	Down ? ItemDimensionY = ItemDimensions.Y - 1 : ItemDimensionY = ItemDimensions.Y;

	FIntPoint ItemPoint = FIntPoint(ItemDimensionX, ItemDimensionY) / 2;

	FVector2D MouseTile = MousePos / TileSize;
	FIntPoint MousePoint = FIntPoint(MouseTile.X, MouseTile.Y);

	DraggedItemTile = MousePoint - ItemPoint;
	
	return true;
}

// Checks if there is room available for item at the given index
bool UInventoryGrid::CheckIfRoomAvailable(FItemData& Payload)
{
	FTile Tile = FTile(DraggedItemTile.X, DraggedItemTile.Y);
	int Index = InventoryComp->TileToIndex(Tile);
	if (InventoryComp->CheckIfInventorySpace(Payload, Index))
	{
		return true;
	}
	return false;
}

bool UInventoryGrid::MousePosInTile(FVector2D MousePos, bool& Right, bool& Down)
{
	if ((FMath::RoundToInt(MousePos.X) % FMath::RoundToInt(TileSize)) > TileSize / 2.0f)
	{
		Right = true;
	}

	if ((FMath::RoundToInt(MousePos.Y) % FMath::RoundToInt(TileSize)) > TileSize / 2.0f)
	{
		Down = true;
	}
	
	return true;
}

// Initializes grid by creating the lines, setting the tile size and refreshing the tiles
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
		}
	}
	
	CreateLineSegments();
	Refresh();
	InventoryComp->OnInventoryChanged.AddDynamic(this, &UInventoryGrid::Refresh);
}

// Adds the coordinates of each line to an array
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

// Refresh each tile and its data
void UInventoryGrid::Refresh()
{
	GridCanvasPanel->ClearChildren();
	TMap<FItemData*, FTile> Items = InventoryComp->GetAllItems();

	for (TMap<FItemData*, FTile>::TIterator It(Items); It; ++It)
	{
		if (It.Key()->ItemImage != nullptr)
		{
			ItemSlotWidget = CreateWidget<UItemSlotUI>(GetWorld(), ItemSlotWidgetClass);
			ItemSlotWidget->TileSize = TileSize;
			ItemSlotWidget->ItemData = It.Key();
			ItemSlotWidget->InventoryComponent = InventoryComp;
			ItemSlotWidget->OnRemove.BindUObject(this, &UInventoryGrid::Remove);
			GridCanvasPanel->AddChild(ItemSlotWidget);
			UPanelSlot* ItemSlot = ItemSlotWidget->Slot;
			if (ItemSlot)
			{
				if (UCanvasPanelSlot* CanvasItemSlot = Cast<UCanvasPanelSlot>(ItemSlot))
				{
					CanvasItemSlot->SetAutoSize(true);
					CanvasItemSlot->SetPosition(FVector2D(It.Value().X * TileSize, It.Value().Y * TileSize));
				}
			}
		}
	}
}

void UInventoryGrid::Remove(FItemData& Item)
{
	InventoryComp->RemoveItem(Item);
}
