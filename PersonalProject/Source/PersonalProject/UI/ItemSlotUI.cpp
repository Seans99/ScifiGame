#include "ItemSlotUI.h"
#include "InventoryUI.h"
#include "Components/Button.h"
#include "PersonalProject/Structs/ItemStruct.h"

void UItemSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UItemSlotUI::InitializeSlot, 0.3f, false);
}

void UItemSlotUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	//ItemButton->OnHovered.AddDynamic(this, &UItemSlotUI::ItemHovered);
	//ItemButton->OnUnhovered.AddDynamic(this, &UItemSlotUI::ItemUnhovered);
	//ItemButton->OnClicked.AddDynamic(this, &UItemSlotUI::ItemMenu);

	//UseBtn->OnClicked.AddDynamic(this, &UItemSlotUI::UseItem);
}

void UItemSlotUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UItemSlotUI::RefreshSlot()
{
	if (ItemData.ItemImage != nullptr)
	{
		Size = FVector2D(ItemData.GridDimensions.X * TileSize, ItemData.GridDimensions.Y * TileSize);
		ItemSlotBox->SetWidthOverride(Size.X);
		ItemSlotBox->SetHeightOverride(Size.Y);

		InventorySlotImage->SetDesiredSizeOverride(Size);
		InventorySlotImage->SetBrushFromTexture(ItemData.ItemImage);
		if (ItemData.bItemStackable)
		{
			FText ItemAmountText = FText::FromString(FString::FromInt(ItemData.ItemAmount));
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

void UItemSlotUI::InitializeSlot()
{
	RefreshSlot();
}
