#include "ItemSlotUI.h"

#include <string>

#include "PersonalProject/Structs/ItemStruct.h"

void UItemSlotUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UItemSlotUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UItemSlotUI::RefreshSlot(FItemData RefreshItem)
{
	if (Item->ItemImage != nullptr)
	{
		InventorySlotImage->SetBrushFromTexture(Item->ItemImage);
		if (Item->bItemStackable)
		{
			FText ItemAmountText = FText::FromString(FString::FromInt(Item->ItemAmount));
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
