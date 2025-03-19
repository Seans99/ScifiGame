#pragma once

#include "CoreMinimal.h"
#include "GUIBase.h"
#include "ItemSlotUI.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "InventoryUI.generated.h"

class UInventoryComponent;

UCLASS()
class PERSONALPROJECT_API UInventoryUI : public UGUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void RefreshInventory(UInventoryComponent* InventoryComp);
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemDesc;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWrapBox* InventoryWrapBox;

	FText ItemNameText;
	FText ItemDescText;

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UItemSlotUI> ItemSlotWidgetClass;

	UPROPERTY()
	UItemSlotUI* ItemSlotWidget;

};
