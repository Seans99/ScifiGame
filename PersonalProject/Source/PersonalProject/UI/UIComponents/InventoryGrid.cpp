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
		LayerId + 1,
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

bool UInventoryGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UCustomDragAndDropOperation* Operation = Cast<UCustomDragAndDropOperation>(InOperation);
	
	if (CheckIfRoomAvailable(GetPayLoad(Operation)))
	{
		FTile Tile = FTile(DraggedItemTile.X, DraggedItemTile.Y);
		int Index = InventoryComp->TileToIndex(Tile);
		InventoryComp->AddItemToInventoryArray(*GetPayLoad(Operation), Index);
	}
	else
	{
		if (!InventoryComp->AddToInventory(*GetPayLoad(Operation)))
		{
			if (UDropAtPlayer* DropAtPlayer = GetGameInstance()->GetSubsystem<UDropAtPlayer>())
			{
				DropAtPlayer->Drop(InOperation->Payload, GetPayLoad(Operation));
			}
		}
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

bool UInventoryGrid::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UCustomDragAndDropOperation* Operation = Cast<UCustomDragAndDropOperation>(InOperation);

	FVector2D MousePos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

	bool Right = false;
	bool Down = false;

	MousePosInTile(MousePos, Right, Down);
	FVector2D ItemDimensions = GetPayLoad(Operation)->GridDimensions;
	int ItemDimensionX = ItemDimensions.X;
	int ItemDimensionY = ItemDimensions.Y;

	Right ? ItemDimensionX = ItemDimensions.X - 1 : ItemDimensionX = ItemDimensions.X;
	Down ? ItemDimensionY = ItemDimensions.Y - 1 : ItemDimensionY = ItemDimensions.Y;

	FIntPoint ItemPoint = FIntPoint(ItemDimensionX, ItemDimensionY) / 2;

	FVector2D MouseTile = MousePos / TileSize;
	FIntPoint MousePoint = FIntPoint(MouseTile.X, MouseTile.Y);

	DraggedItemTile = MousePoint - ItemPoint;
	
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, Operation);
}

FItemData* UInventoryGrid::GetPayLoad(UCustomDragAndDropOperation* DragDropOperation)
{
	if (DragDropOperation != nullptr)
	{
		return &DragDropOperation->ItemData;
	}

	return nullptr;
}

bool UInventoryGrid::CheckIfRoomAvailable(FItemData* Payload)
{
	if (Payload != nullptr)
	{
		FTile Tile = FTile(DraggedItemTile.X, DraggedItemTile.Y);
		int Index = InventoryComp->TileToIndex(Tile);
		if (InventoryComp->CheckIfInventorySpace(*Payload, Index))
		{
			UE_LOG(LogTemp, Warning, TEXT("There is room available"));
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("There is NO room available"));
	return false;
}

bool UInventoryGrid::MousePosInTile(FVector2D MousePos, bool& Right, bool& Down)
{
	int TSize = TileSize;
	int X = MousePos.X;
	int Y = MousePos.Y;
	
	if ((X % TSize) > TileSize / 2.0f)
	{
		Right = true;
	}

	if ((Y % TSize) > TileSize / 2.0f)
	{
		Down = true;
	}
	
	return true;
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
		}
	}
	
	CreateLineSegments();
	Refresh();
	InventoryComp->OnInventoryChanged.AddDynamic(this, &UInventoryGrid::Refresh);
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
