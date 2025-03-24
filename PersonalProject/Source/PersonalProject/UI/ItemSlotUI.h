#pragma once

#include "CoreMinimal.h"
#include "GUIBase.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "PersonalProject/Structs/ItemStruct.h"
#include "ItemSlotUI.generated.h"

class UInventoryUI;

UCLASS()
class PERSONALPROJECT_API UItemSlotUI : public UGUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void RefreshSlot(FItemData Item);

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* InventorySlotImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* ItemAmountBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemAmount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* ItemUseBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* UseBtn;

public:
	UFUNCTION()
	void ItemHovered();

	UFUNCTION()
	void ItemUnhovered();

	UFUNCTION()
	void ItemMenu();

	UFUNCTION()
	void UseItem();

public:
	UPROPERTY()
	UInventoryUI* InventoryUI;

	FItemData ItemData;
	
	int Index;

};
