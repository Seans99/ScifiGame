#include "InventoryUI.h"
#include "PersonalProject/Components/InventoryComponent.h"
#include "VisualizeTexture.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerController.h"

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	Controller = Cast<APrimaryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	
	CloseBtn->OnClicked.AddDynamic(this, &UInventoryUI::CloseInventory);
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
			ItemSlotWidget->Index = i;
			ItemSlotWidget->RefreshSlot(InventoryComp->Items[i]);
			InventoryWrapBox->AddChild(ItemSlotWidget);
		}
	}
}

void UInventoryUI::CloseInventory()
{
	Controller->DisableMouse();
	this->SetVisibility(ESlateVisibility::Hidden);
}
