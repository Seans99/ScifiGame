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

bool UInventoryComponent::AddToInventory(AItemBase* InteractedItem)
{
	if (InteractedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractedItem is null!"));
		return false;
	}
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		bool bCanAdd = false;

		if (InteractedItem->ItemData.bItemStackable)
		{
			if (Items[i].ItemName.EqualTo(InteractedItem->ItemData.ItemName))
			{
				if (CheckIfStackable(Items[i], InteractedItem))
				{
					bCanAdd = true;
				}
			}
			else
			{
				if (CheckIfInventorySpace(InteractedItem->ItemData, i))
				{
					AddItemToInventoryArray(InteractedItem->ItemData, i);
					bCanAdd = true;
				}
			}
		}
		else
		{
			if (CheckIfInventorySpace(InteractedItem->ItemData, i))
			{
				AddItemToInventoryArray(InteractedItem->ItemData, i);
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

bool UInventoryComponent::CheckIfStackable(FItemData& Item, AItemBase* InteractedItem)
{
	if (Item.ItemAmount < MaxAmountPerItem)
	{
		int SumItems = Item.ItemAmount + InteractedItem->ItemData.ItemAmount;
		if (SumItems <= MaxAmountPerItem)
		{
			Item.ItemAmount = SumItems;
			return true;
		}
		else
		{
			int LeftOver = SumItems - MaxAmountPerItem;
			Item.ItemAmount = MaxAmountPerItem;
			InteractedItem->ItemData.ItemAmount = LeftOver;
			return false;
		}
	}
	return false;
}

bool UInventoryComponent::CheckIfInventorySpace(FItemData& Item, int Index)
{
	TArray<FTile> Tiles = ForEachIndex(Item, Index);
	for (int i = 0; i < Tiles.Num(); ++i)
	{
		if (Tiles[i].X >= 0 && Tiles[i].Y >= 0 && Tiles[i].X < Columns && Tiles[i].Y < Rows)
		{
			int TileIndex = TileToIndex(Tiles[i]);
			FItemData ItemData;
			bool bHasFoundItem = GetItemAtIndex(TileIndex, ItemData);
			if (bHasFoundItem)
			{
				if (ItemData.bInInventory)
				{
					return false;
				}
			}
			else
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


//Get X and Y coordinate to Tile
FTile UInventoryComponent::IndexToTile(int Index) const
{
	int X = Index % Columns;
	int Y = Index / Columns;

	return FTile(X, Y);
}

int UInventoryComponent::TileToIndex(FTile Tile) const
{
	int Index = Tile.X + (Tile.Y * Columns);
	return Index;
}

bool UInventoryComponent::GetItemAtIndex(int Index, FItemData& Item)
{
	if (Items.IsValidIndex(Index))
	{
		Item = Items[Index];
		return true;
	}

	Item = FItemData();
	return false;
}

void UInventoryComponent::AddItemToInventoryArray(FItemData& Item, int Index)
{
	TArray<FTile> Tiles = ForEachIndex(Item, Index);
	UE_LOG(LogTemp, Warning, TEXT("Tiles: %d"), Tiles.Num());
	for (int i = 0; i < Tiles.Num(); ++i)
	{
		int TileIndex = TileToIndex(Tiles[i]);
		if (i == 0)
		{
			if (Items.IsValidIndex(TileIndex))
			{
				Item.ItemID = Index;
				Item.bInInventory = true;
				Items[TileIndex] = Item;
			}
		}
		else
		{
			Items[TileIndex].bInInventory = true;
		}
	}
}

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
					if (j == 0)
					{
						if (Items.IsValidIndex(TileIndex))
						{
							Items[TileIndex].ItemID = 0;
							Items[TileIndex].bInInventory = false;
							Items[TileIndex] = FItemData();
						}
					}
					else
					{
						Items[TileIndex].bInInventory = false;
					}
				}
			}
		}
		bIsDirty = true;
	}
}

UInventoryUI* UInventoryComponent::GetInventoryWidget()
{
	return InventoryWidget;
}

TMap<FItemData*, FTile> UInventoryComponent::GetAllItems()
{
	TMap<FItemData*, FTile> AllItems;
	FItemData* CurrentItem;
	for (int i = 0; i < Items.Num(); i++)
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
