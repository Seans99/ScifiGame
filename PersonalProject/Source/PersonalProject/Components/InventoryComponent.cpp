#include "InventoryComponent.h"

#include "Kismet/GameplayStatics.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerCharacter.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerController.h"
#include "PersonalProject/UI/InventoryUI.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APrimaryPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerController = Cast<APrimaryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	InventoryWidget = CreateWidget<UInventoryUI>(GetWorld(), InventoryWidgetClass);

	if (Player)
	{
		UE_LOG(LogTemp, Display, TEXT("Player Valid"));
		Player->OnInventory.AddDynamic(this, &UInventoryComponent::OpenInventory);
	}
	
	Items.SetNum(InventorySize);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::OpenInventory()
{
	if (!InventoryWidget->IsInViewport())
	{
		InventoryWidget->AddToViewport();
		PlayerController->EnableMouse();
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->EnableMouse();
	}
}

