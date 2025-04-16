#include "InventoryComponent.h"

#include "IContentBrowserSingleton.h"
#include "Kismet/GameplayStatics.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerCharacter.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerController.h"
#include "PersonalProject/UI/InventoryUI.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APrimaryPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerController = Cast<APrimaryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	Items.SetNum((Columns * Rows));
	
	InventoryWidget = CreateWidget<UInventoryUI>(GetWorld(), InventoryWidgetClass);
	InventoryWidget->InventoryComponent = this;
	InventoryWidget->TileSize = TileSize;

	if (Player)
	{
		UE_LOG(LogTemp, Display, TEXT("Player Valid"));
		Player->OnInventory.AddDynamic(this, &UInventoryComponent::OpenInventory);
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsDirty)
	{
		if (OnInventoryChanged.IsBound()) OnInventoryChanged.Broadcast();
		bIsDirty = false;
	}
}

// Open inventory widget
void UInventoryComponent::OpenInventory()
{
	if (!InventoryWidget->IsInViewport())
	{
		InventoryWidget->AddToViewport();
		PlayerController->EnableMouse();
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->EnableMouse();
	}
}

// Try to add item to inventory
bool UInventoryComponent::AddToInventory(FItemData InteractedItem)
{
	if (!InteractedItem.ItemImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractedItem is null!"));
		return false;
	}
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		bool bCanAdd = false;

		if (InteractedItem.bItemStackable)
		{
			if (CheckIfStackable(InteractedItem))
			{
				bCanAdd = true;
			}
			else
			{
				if (CheckIfInventorySpace(InteractedItem, i))
				{
					AddItemToInventoryArray(InteractedItem, i);
					bCanAdd = true;
				}
			}
		}
		else
		{
			if (CheckIfInventorySpace(InteractedItem, i))
			{
				AddItemToInventoryArray(InteractedItem, i);
				bCanAdd = true;
			}
		}

		if (bCanAdd)
		{
			bIsDirty = true;
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::CheckIfStackable(FItemData& InteractedItem)
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemName.EqualTo(InteractedItem.ItemName))
		{
			if (Items[i].ItemAmount < MaxAmountPerItem)
			{
				int SumItems = Items[i].ItemAmount + InteractedItem.ItemAmount;
				if (SumItems <= MaxAmountPerItem)
				{
					Items[i].ItemAmount = SumItems;
					return true;
				}
				else
				{
					int LeftOver = SumItems - MaxAmountPerItem;
					Items[i].ItemAmount = MaxAmountPerItem;
					InteractedItem.ItemAmount = LeftOver;
					return false;
				}
			}
		}
	}
	
	return false;
}

bool UInventoryComponent::CheckIfInventorySpace(FItemData& Item, int Index)
{
	TArray<FTile> Tiles = ForEachIndex(Item, Index);
	for (int i = 0; i < Tiles.Num(); ++i)
	{
		// Checks if the tiles are valid (inside the grid)
		if (Tiles[i].X >= 0 && Tiles[i].Y >= 0 && Tiles[i].X < Columns && Tiles[i].Y < Rows)
		{
			int TileIndex = TileToIndex(Tiles[i]);
			if (GetItemAtIndex(TileIndex).bInInventory)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

// Returns all the tiles the item will occupy
TArray<FTile> UInventoryComponent::ForEachIndex(FItemData& Item, int Index)
{
	TArray<FTile> Tiles;
	FTile Tile = IndexToTile(Index);
	FIntPoint Dimensions = Item.GridDimensions;
	FTile ReturnedTile;

	int LastIndexX = Tile.X + Dimensions.X;
	int LastIndexY = Tile.Y + Dimensions.Y;

	for (int i = Tile.X; i < LastIndexX; ++i)
	{
		for (int j = Tile.Y; j < LastIndexY; ++j)
		{
			Tiles.Add(FTile(i, j));
		}
	}

	return Tiles;
}


// Get X and Y coordinate to Tile
FTile UInventoryComponent::IndexToTile(int Index) const
{
	int X = Index % Columns;
	int Y = Index / Columns;

	return FTile(X, Y);
}

// Get index of a Tile
int UInventoryComponent::TileToIndex(FTile Tile) const
{
	int Index = Tile.X + (Tile.Y * Columns);
	return Index;
}

// Get the item at given index
FItemData UInventoryComponent::GetItemAtIndex(int Index)
{
	FItemData ItemData;
	if (Items.IsValidIndex(Index))
	{
		ItemData = Items[Index];
		return ItemData;
	}

	ItemData = FItemData();
	return ItemData;
}

// Add item to the inventory and its array
void UInventoryComponent::AddItemToInventoryArray(FItemData& Item, int Index)
{
	TArray<FTile> Tiles = ForEachIndex(Item, Index);
	for (int i = 0; i < Tiles.Num(); ++i)
	{
		int TileIndex = TileToIndex(Tiles[i]);
		if (Items.IsValidIndex(TileIndex))
		{
			if (i == 0)
			{
				Item.ItemID = Index;
				Item.bInInventory = true;
				Items[TileIndex] = Item;
			}
			else
			{
				Items[TileIndex].ItemID = Index;
				Items[TileIndex].bInInventory = true;
			}
		}
	}
	bIsDirty = true;
}

// Remove item from inventory array
void UInventoryComponent::RemoveItem(FItemData& Item)
{
	if (Item.bInInventory)
	{
		for (int i = 0; i < Items.Num(); ++i)
		{
			if (Items[i].ItemID == Item.ItemID)
			{
				TArray<FTile> Tiles = ForEachIndex(Items[i], i);
				for (int j = 0; j < Tiles.Num(); ++j)
				{
					int TileIndex = TileToIndex(Tiles[j]);
					if (Items.IsValidIndex(TileIndex))
					{
						Items[TileIndex] = FItemData();
					}
				}
				bIsDirty = true;
			}
		}
	}
}

UInventoryUI* UInventoryComponent::GetInventoryWidget()
{
	return InventoryWidget;
}

// Get all the items inside the inventory
TMap<FItemData*, FTile> UInventoryComponent::GetAllItems()
{
	TMap<FItemData*, FTile> AllItems;
	FItemData* CurrentItem;
	for (int i = 0; i < Items.Num(); ++i)
	{
		CurrentItem = &Items[i];
		if (CurrentItem->bInInventory)
		{
			if (!AllItems.Contains(CurrentItem))
			{
				FTile Tile = IndexToTile(i);
				AllItems.Add(CurrentItem, Tile);
			}
		}
	}
	
	return AllItems;
}
