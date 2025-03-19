#include "InventoryUI.h"
#include "PersonalProject/Components/InventoryComponent.h"

#include "VisualizeTexture.h"
#include "Kismet/GameplayStatics.h"

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UInventoryUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryUI::RefreshInventory(UInventoryComponent* InventoryComp)
{
	if (InventoryComp)
	{
		InventoryWrapBox->ClearChildren();
		for (int i = 0; i < InventoryComp->Items.Num(); i++)
		{
			ItemSlotWidget = CreateWidget<UItemSlotUI>(GetWorld(), ItemSlotWidgetClass);
			ItemSlotWidget->Item = &InventoryComp->Items[i];
			ItemSlotWidget->Index = i;
			ItemSlotWidget->RefreshSlot(InventoryComp->Items[i]);
			InventoryWrapBox->AddChild(ItemSlotWidget);
		}
	}
}
