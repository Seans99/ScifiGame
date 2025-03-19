#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PersonalProject/Structs/ItemStruct.h"
#include "InventoryComponent.generated.h"

class APrimaryPlayerCharacter;
class APrimaryPlayerController;

class UInventoryUI;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PERSONALPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UInventoryUI> InventoryWidgetClass;

	UPROPERTY()
	UInventoryUI* InventoryWidget;

public:
	UFUNCTION()
	void ToggleInventory();

public:
	TArray<FItemData> Items;

	int CurrentIndex;
	int MaxAmountPerItem = 10;
	int InventorySize = 36;

private:
	UPROPERTY()
	APrimaryPlayerCharacter* Player;

	UPROPERTY()
	APrimaryPlayerController* PlayerController;
};
