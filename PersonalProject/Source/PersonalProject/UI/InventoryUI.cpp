#include "InventoryUI.h"

#include "ItemSlotUI.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerCharacter.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerController.h"
#include "PersonalProject/PrimarySystems/GameInstances/DropAtPlayer.h"
#include "UIComponents/InventoryGrid.h"

void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInventoryUI::InitializeInventory, 0.1f, false);
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

// Drops item when dropped outside of inventory grid
bool UInventoryUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (!InOperation->Payload)
	{
		UE_LOG(LogTemp, Error, TEXT("No Payload"));
	}

	UCustomDragAndDropOperation* Operation = Cast<UCustomDragAndDropOperation>(InOperation);
	
	DropItemAtPlayer(InOperation->Payload, &Operation->ItemData);
	
	return true;
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

void UInventoryUI::DropItemAtPlayer(UObject* ItemToDrop, FItemData* ItemData)
{
	if (ItemToDrop == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemToDrop is NULL"));
	}

	// Drop item on ground in front of player
	if (UDropAtPlayer* DropAtPlayer = GetGameInstance()->GetSubsystem<UDropAtPlayer>())
	{
		DropAtPlayer->Drop(ItemToDrop, ItemData);
	}
}


