#include "InventoryUI.h"

#include "ItemSlotUI.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "PersonalProject/Components/InventoryComponent.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerCharacter.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerController.h"
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

bool UInventoryUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (!InOperation->Payload)
	{
		UE_LOG(LogTemp, Error, TEXT("No Payload"));
	}
	
	UItemSlotUI* DroppedItem = Cast<UItemSlotUI>(InOperation->Payload);
	DropItemAtPlayer(InOperation->Payload);
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
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

void UInventoryUI::DropItemAtPlayer(UObject* ItemToDrop)
{
	if (ItemToDrop == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemToDrop is NULL"));
	}
	
	APrimaryPlayerCharacter* Player = Cast<APrimaryPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector Location = Player->GetActorLocation() + (Player->GetActorForwardVector() * 150.f);
	FRotator Rotation = Player->GetActorRotation();

	FHitResult Hit;
	FVector EndLocation = Location - FVector(0,0,1000);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Location, EndLocation, ECollisionChannel::ECC_Visibility))
	{
		Location = Hit.Location;
	}

	UBlueprint* ItemBlueprint = Cast<UBlueprint>(ItemToDrop);
	UClass* ItemClass = ItemBlueprint->GeneratedClass;
	
	AActor* SpawnItem = GetWorld()->SpawnActor<AActor>(ItemClass, Location, Rotation);
}


