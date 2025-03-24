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

	if (Player)
	{
		UE_LOG(LogTemp, Display, TEXT("Player Valid"));
		Player->OnInventory.AddDynamic(this, &UInventoryComponent::OpenInventory);
	}

	Items.SetNum(MaxInventorySize);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::OpenInventory()
{
	if (!InventoryWidget->IsInViewport())
	{
		InventoryWidget->AddToViewport();
		InventoryWidget->RefreshInventory(this);
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
	UE_LOG(LogTemp, Display, TEXT("AddToInventory"));
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
			InventoryWidget->RefreshInventory(this);
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
	UE_LOG(LogTemp, Display, TEXT("CheckIfInventorySpace"));
	Items.Insert(Item, CurrentIndex);
	Items.RemoveAt((MaxInventorySize - CurrentIndex));
	return true;
}

