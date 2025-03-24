#include "ItemSlotUI.h"

#include <string>

#include "IDetailTreeNode.h"
#include "InventoryUI.h"
#include "Components/Button.h"
#include "PersonalProject/Structs/ItemStruct.h"

void UItemSlotUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	ItemButton->OnHovered.AddDynamic(this, &UItemSlotUI::ItemHovered);
	ItemButton->OnUnhovered.AddDynamic(this, &UItemSlotUI::ItemUnhovered);
	ItemButton->OnClicked.AddDynamic(this, &UItemSlotUI::ItemMenu);

	UseBtn->OnClicked.AddDynamic(this, &UItemSlotUI::UseItem);
}

void UItemSlotUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UItemSlotUI::RefreshSlot(FItemData Item)
{
	if (Item.ItemImage != nullptr)
	{
		ItemData = Item;
		InventorySlotImage->SetBrushFromTexture(Item.ItemImage);
		if (Item.bItemStackable)
		{
			FText ItemAmountText = FText::FromString(FString::FromInt(Item.ItemAmount));
			ItemAmount->SetText(ItemAmountText);
		}
		else
		{
			ItemAmountBorder->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		ItemAmountBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemSlotUI::ItemHovered()
{
	InventoryUI->SetItemDetails(ItemData.ItemName, ItemData.ItemDesc);
}

void UItemSlotUI::ItemUnhovered()
{
	if (ItemUseBox->IsVisible())
	{
		ItemUseBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemSlotUI::ItemMenu()
{
	if (ItemData.ItemImage)
	{
		ItemUseBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemSlotUI::UseItem()
{
	// Use item
}
