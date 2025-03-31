#include "InventoryUI.h"

#include "ItemSlotUI.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "PersonalProject/Components/InventoryComponent.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerController.h"
#include "UIComponents/InventoryGrid.h"

void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInventoryUI::InitializeInventory, 0.3f, false);
}

void UInventoryUI::InitializeInventory()
{
	InventoryGrid->InitializeGrid(InventoryComponent, TileSize);
}

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

void UInventoryUI::CloseInventory()
{
	Controller->DisableMouse();
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryUI::SetItemDetails(FText Name, FText Desc)
{
	ItemName->SetText(Name);
	ItemDesc->SetText(Desc);

	// Force immediate update
	ItemName->SynchronizeProperties();
	ItemDesc->SynchronizeProperties();
}


