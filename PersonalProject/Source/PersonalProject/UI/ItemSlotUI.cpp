#include "ItemSlotUI.h"
#include "InventoryUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
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
	}
}

void UItemSlotUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UItemSlotUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	UDragDropOperation* DragDrop = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	DragDrop->Payload = ItemData->ItemObjectReference;
	DragDrop->DefaultDragVisual = this;
	DragDrop->Pivot = EDragPivot::MouseDown;
	OutOperation = DragDrop;

	FText Name = FText::FromString("");
	FText Desc = FText::FromString("");
	InventoryUI->SetItemDetails(Name, Desc);	

	OnRemove.ExecuteIfBound(*ItemData);
	RemoveFromParent();
}

FReply UItemSlotUI::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& InMouseEvent)
{
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UItemSlotUI::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	InventoryUI->SetItemDetails(ItemData->ItemName, ItemData->ItemDesc);
	BackgroundImage->SetVisibility(ESlateVisibility::Visible);
}

void UItemSlotUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	FText Name = FText::FromString("");
	FText Desc = FText::FromString("");
	InventoryUI->SetItemDetails(Name, Desc);
	BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
}

void UItemSlotUI::RefreshSlot()
{
	if (ItemData->bInInventory)
	{
		Size = FVector2D((ItemData->GridDimensions.X * TileSize), (ItemData->GridDimensions.Y * TileSize));
		ItemSlotBox->SetWidthOverride(Size.X);
		ItemSlotBox->SetHeightOverride(Size.Y);

		InventorySlotImage->SetDesiredSizeOverride(Size);
		InventorySlotImage->SetBrushFromTexture(ItemData->ItemImage);
		if (ItemData->bItemStackable)
		{
			FText ItemAmountText = FText::FromString(FString::FromInt(ItemData->ItemAmount));
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

void UItemSlotUI::InitializeSlot()
{
	RefreshSlot();
}
