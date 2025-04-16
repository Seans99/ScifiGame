#include "ItemBase.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PersonalProject/Components/InventoryComponent.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerCharacter.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	RootComponent = ItemMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnSphereEndOverlap);
	SphereComponent->SetupAttachment(ItemMesh);

	KeyPrompt = CreateDefaultSubobject<UWidgetComponent>("KeyPrompt");
	KeyPrompt->SetupAttachment(ItemMesh);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APrimaryPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->OnInteract.AddDynamic(this, &AItemBase::PickupItem);
	}

	if (ItemData.ItemMesh)
	{
		ItemMesh->SetStaticMesh(ItemData.ItemMesh);
	}
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bCanInteract)
	{
		KeyPrompt->SetVisibility(true);
	}
	else
	{
		KeyPrompt->SetVisibility(false);
	}

	// Rotates the prompt for which key to press for interacting with the item towards the player.
	if (KeyPrompt->IsVisible())
	{
		FVector Start = KeyPrompt->GetComponentLocation();
		FVector Target = Player->GetActorLocation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
		
		KeyPrompt->SetWorldRotation(LookAtRotation);
	}

	bCanInteract = false;
}

FItemData AItemBase::GetItemData()
{
	return ItemData;
}

// Checks if player is in range and can interact
// If true tries adding the item to the inventory
// If Success destroy the actor
void AItemBase::PickupItem()
{
	if (bPlayerInRange && bCanInteract)
	{
		if (Player->FindComponentByClass<UInventoryComponent>()->AddToInventory(ItemData))
		{
			Destroy();
		}
	}
}

void AItemBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APrimaryPlayerCharacter>(OtherActor))
	{
		bPlayerInRange = true;
	}
}

void AItemBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APrimaryPlayerCharacter>(OtherActor))
	{
		bPlayerInRange = false;
	}
}


