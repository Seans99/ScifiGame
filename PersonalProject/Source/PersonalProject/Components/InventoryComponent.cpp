#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APrimaryPlayerCharacter>(GetOwner());
	if (Player)
	{
		Player->OnInventory.AddDynamic(this, &UInventoryComponent::ToggleInventory);
	}
	
	InventoryWidget = CreateWidget<UInventoryUI>(GetWorld(), InventoryWidgetClass);
	Items.SetNum(InventorySize);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::ToggleInventory()
{
	if (!InventoryOpen)
	{
		InventoryOpen = true;
		if (!InventoryWidget->IsInViewport())
		{
			InventoryWidget->AddToViewport();
		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		InventoryOpen = false;
		if (InventoryWidget->IsInViewport())
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

