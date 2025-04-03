#pragma once

#include "CoreMinimal.h"
#include "GUIBase.h"
#include "PersonalProject/Structs/ItemStruct.h"
#include "InventoryUI.generated.h"

class UInventoryComponent;
class UInventoryGrid;
class APrimaryPlayerController;
class UItemSlotUI;

class UTextBlock;
class UButton;
class UWrapBox;

UCLASS()
class PERSONALPROJECT_API UInventoryUI : public UGUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation) override;
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemDesc;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CloseBtn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UInventoryGrid* InventoryGrid;

public:
	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	int TileSize;
	
public:
	UFUNCTION()
	void CloseInventory();

public:
	void SetItemDetails(FText Name, FText Desc);
	void DropItemAtPlayer(UObject* ItemToDrop);

private:
	UFUNCTION()
	void InitializeInventory();

private:
	UPROPERTY()
	APrimaryPlayerController* Controller;

	FTimerHandle TimerHandle;
};
