#include "InventoryComponent.h"
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

	
	for (int i = 0; i < Items.Num(); i++)
	{
		CurrentIndex = i;
		bool bCanAdd = false;

		UE_LOG(LogTemp, Display, TEXT("Looping through inventory: %d, Checking item: %s"), i, *InteractedItem->ItemData.ItemName.ToString());

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
				if (CheckIfInventorySpace(InteractedItem->ItemData))
				{
					AddItemToInventoryArray(InteractedItem->ItemData);
					bCanAdd = true;
				}
			}
		}
		else
		{
			if (CheckIfInventorySpace(InteractedItem->ItemData))
			{
				AddItemToInventoryArray(InteractedItem->ItemData);
				bCanAdd = true;
			}
		}

		if (bCanAdd)
		{
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

bool UInventoryComponent::CheckIfInventorySpace(FItemData& Item)
{
	TArray<FTile> Tiles = ForEachIndex(Item);
	for (auto Tile : Tiles)
	{
		if (Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Tile.Y < Rows)
		{
			int Index = TileToIndex(Tile);
			FItemData ItemData;
			bool bHasFoundItem = GetItemAtIndex(Index, ItemData);
			if (bHasFoundItem)
			{
				if (ItemData.ItemImage != nullptr)
				{
					
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

TArray<FTile> UInventoryComponent::ForEachIndex(FItemData& Item)
{
	TArray<FTile> Tiles; 

	FTile Tile = IndexToTile(CurrentIndex);
	FIntPoint Dimensions = Item.GridDimensions;

	int LastIndexX = Tile.X + (Dimensions.X - 1);
	int LastIndexY = Tile.Y + (Dimensions.Y - 1);

	for (int i = Tile.X; i < LastIndexX; i++)
	{
		for (int j = Tile.Y; j < LastIndexY; j++)
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

	Item = FItemData();
	return false;
}

void UInventoryComponent::AddItemToInventoryArray(FItemData& Item)
{
	TArray<FTile> Tiles = ForEachIndex(Item);
	for (auto Tile : Tiles)
	{
		int Index = TileToIndex(Tile);
		if (Items.IsValidIndex(Index))
		{
			Items[Index] = Item;
		}
	}
	bIsDirty = true;
}

void UInventoryComponent::RemoveItem(FItemData& Item)
{
	
}

TArray<FItemData> UInventoryComponent::GetAllItems()
{
	return Items;
}
