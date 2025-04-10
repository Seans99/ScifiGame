#include "DropAtplayer.h"
#include "Kismet/GameplayStatics.h"
#include "PersonalProject/PrimarySystems/PrimaryPlayerCharacter.h"

void UDropAtPlayer::Drop(UObject* ItemToDrop, FItemData* ItemData)
{
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

    if (ItemData->bItemStackable)
    {
        for (int i = 0; i < ItemData->ItemAmount; ++i)
        {
            GetWorld()->SpawnActor<AActor>(ItemClass, Location, Rotation);
        }
    }
    else
    {
        GetWorld()->SpawnActor<AActor>(ItemClass, Location, Rotation);
    }
}
