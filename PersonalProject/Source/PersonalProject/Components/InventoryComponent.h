#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PersonalProject/Actors/Items/ItemBase.h"
#include "PersonalProject/Structs/ItemStruct.h"
#include "PersonalProject/Structs/Tile.h"
#include "InventoryComponent.generated.h"

class APrimaryPlayerCharacter;
class APrimaryPlayerController;

class UInventoryUI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedSignature);

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
	void OpenInventory();

	UFUNCTION()
	bool AddToInventory(FItemData InteractedItem);

	TMap<FItemData*, FTile> GetAllItems();

	bool CheckIfStackable(FItemData& Item, FItemData& InteractedItem);
	bool CheckIfInventorySpace(FItemData& Item, int Index);
	TArray<FTile> ForEachIndex(FItemData& Item, int Index);
	FTile IndexToTile(int Index) const;
	int TileToIndex(FTile Tile) const;
	bool GetItemAtIndex(int Index, FItemData& Item);
	void AddItemToInventoryArray(FItemData& Item, int Index);
	void RemoveItem(FItemData& Item);

	UInventoryUI* GetInventoryWidget();

public:
	FOnInventoryChangedSignature OnInventoryChanged;

private:
	TArray<FItemData> Items;

public:
	int MaxAmountPerItem = 10;
	int MaxInventorySize;

	UPROPERTY(EditAnywhere)
	int TileSize = 100;
	UPROPERTY(EditAnywhere)
	int Columns = 8;
	UPROPERTY(EditAnywhere)
	int Rows = 4;

private:
	bool bIsDirty = false;

private:
	UPROPERTY()
	APrimaryPlayerCharacter* Player;

	UPROPERTY()
	APrimaryPlayerController* PlayerController;
};
