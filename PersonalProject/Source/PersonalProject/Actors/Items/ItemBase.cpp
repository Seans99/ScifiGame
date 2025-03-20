#include "ItemBase.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
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
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FItemData AItemBase::GetItemData()
{
	return ItemData;
}

void AItemBase::PickupItem()
{
	if (bPlayerInRange)
	{
		Player->FindComponentByClass<UInventoryComponent>()->AddToInventory(GetItemData());
		Destroy();
	}
}

void AItemBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APrimaryPlayerCharacter>(OtherActor))
	{
		KeyPrompt->SetVisibility(true);
		bPlayerInRange = true;
	}
}

void AItemBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APrimaryPlayerCharacter>(OtherActor))
	{
		KeyPrompt->SetVisibility(false);
		bPlayerInRange = false;
	}
}


