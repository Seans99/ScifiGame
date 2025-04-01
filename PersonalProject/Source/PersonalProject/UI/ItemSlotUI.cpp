#include "ItemSlotUI.h"
#include "InventoryUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PersonalProject/Components/InventoryComponent.h"
#include "PersonalProject/Structs/ItemStruct.h"

void UItemSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UItemSlotUI::InitializeSlot, 0.1f, false);
}

void UItemSlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryComponent != nullptr)
	{
		InventoryUI = InventoryComponent->GetInventoryWidget();

		ItemButton->OnHovered.AddDynamic(this, &UItemSlotUI::ItemHovered);
		ItemButton->OnUnhovered.AddDynamic(this, &UItemSlotUI::ItemUnhovered);
	}
}

void UItemSlotUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UItemSlotUI::RefreshSlot()
{
	if (ItemData.bInInventory)
	{
		Size = FVector2D((ItemData.GridDimensions.X * TileSize), (ItemData.GridDimensions.Y * TileSize));
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
	FText Name = FText::FromString("");
	FText Desc = FText::FromString("");
	InventoryUI->SetItemDetails(Name, Desc);
}

void UItemSlotUI::InitializeSlot()
{
	RefreshSlot();
}
