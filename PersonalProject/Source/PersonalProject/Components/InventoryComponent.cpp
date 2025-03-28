#include "InventoryComponent.h"

#include <functional>

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

	InventoryWidget = CreateWidget<UInventoryUI>(GetWorld(), InventoryWidgetClass);
	InventoryWidget->InventoryComponent = this;
	InventoryWidget->TileSize = TileSize;

	if (Player)
	{
		UE_LOG(LogTemp, Display, TEXT("Player Valid"));
		Player->OnInventory.AddDynamic(this, &UInventoryComponent::OpenInventory);
	}

	int size = Columns * Rows;
	Items.SetNum(size);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsDirty)
	{
		if (OnInventoryChanged.IsBound()) OnInventoryChanged.Broadcast();
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

void UInventoryComponent::AddToInventory(AItemBase* InteractedItem)
{
	int Index = 0;
	for (auto& Item : Items)
	{
		CurrentIndex = Index;
		bool bCanAdd = false;
		if (InteractedItem->ItemData.bItemStackable)
		{
			if (Item.ItemName.EqualTo(InteractedItem->ItemData.ItemName))
			{
				if (CheckIfStackable(Item, InteractedItem))
				{
					bCanAdd = true;
				}
				else
				{
					if (CheckIfInventorySpace(InteractedItem->ItemData))
					{
						bCanAdd = true;
					}
				}
			}
			else
			{
				if (CheckIfInventorySpace(InteractedItem->ItemData))
				{
					bCanAdd = true;
				}
			}
		}
		else
		{
			if (CheckIfInventorySpace(InteractedItem->ItemData))
			{
				bCanAdd = true;
			}
		}

		if (bCanAdd)
		{
			InteractedItem->Destroy();
			Index++;
			bCanAdd = false;
			break;
		}
	}
}

bool UInventoryComponent::CheckIfStackable(FItemData& Item, AItemBase* InteractedItem)
{
	UE_LOG(LogTemp, Display, TEXT("CheckIfStackable"));
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

bool UInventoryComponent::CheckIfInventorySpace(FItemData& Item)
{
	if (Items.IsValidIndex(MaxInventorySize - 1) && Items[MaxInventorySize - 1].ItemImage)
	{
		return false;
	}

	FTile ItemTile = ForEachIndex(Item, CurrentIndex);
	if (ItemTile.X >= 0 && ItemTile.Y >= 0 && ItemTile.X < Columns && ItemTile.Y < Rows)
	{
		int Index = TileToIndex(ItemTile);
		FItemData ItemData;
		bool bHasFoundItem = GetItemAtIndex(Index, ItemData);
		if (bHasFoundItem)
		{
			bHasFoundItem = false;
			for (auto i : Items)
			{
				if (i.ItemName.EqualTo(ItemData.ItemName))
				{
					bHasFoundItem = true;
				}
			}
			if (!bHasFoundItem)
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

	AddItemToInventoryArray(Item);
	return true;
}

FTile UInventoryComponent::ForEachIndex(FItemData& Item, int Index)
{
	FTile Tile = IndexToTile(Index);
	FIntPoint Dimensions = Item.GridDimensions;
	int FirstIndexX = Tile.X;
	int LastIndexX = FirstIndexX + (Dimensions.X - 1);
	int FirstIndexY = Tile.Y;
	int LastIndexY = FirstIndexY + (Dimensions.Y - 1);
	FTile ItemTile;

	for (int i = FirstIndexX; i < LastIndexX; i++)
	{
		for (int j = FirstIndexY; j < LastIndexY; j++)
		{
			ItemTile = FTile(i, j);
		}
	}

	return ItemTile;
}


//Get X and Y coordinate to Tile
FTile UInventoryComponent::IndexToTile(int Index) const
{
	int X = Index % Columns;
	int Y = Index / Columns;

	FTile Tile = FTile(X, Y);
	return Tile;
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

	return false;
}

void UInventoryComponent::AddItemToInventoryArray(FItemData& Item)
{
	FTile Tile = ForEachIndex(Item, CurrentIndex);
	int Index = TileToIndex(Tile);
	Items.Insert(Item, Index);
	bIsDirty = true;
}

void UInventoryComponent::RemoveItem(FItemData& Item)
{
	
}

TArray<FItemData> UInventoryComponent::GetAllItems()
{
	TArray<FItemData> AllItems;
	int Index = 0;
	for (auto Item : Items)
	{
		if (!AllItems.Contains(Item))
		{
			AllItems.Add(Item);
			Index++;
		}
	}

	return AllItems;
}
