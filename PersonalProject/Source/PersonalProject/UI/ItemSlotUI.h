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
class UInventoryComponent;

DECLARE_DELEGATE_OneParam(FRemoveSignature, FItemData&);

UCLASS()
class PERSONALPROJECT_API UItemSlotUI : public UGUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void RefreshSlot();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* ItemSlotBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* InventorySlotImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* ItemAmountBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemAmount;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ItemButton;

public:
	UFUNCTION()
	void ItemHovered();

	UFUNCTION()
	void ItemUnhovered();

	UFUNCTION()
	void InitializeSlot();

public:
	FRemoveSignature OnRemove;

public:
	UPROPERTY()
	UInventoryUI* InventoryUI;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	FItemData* ItemData;
	
	int Index;

	float TileSize;

private:
	FTimerHandle TimerHandle;
	
	FVector2D Size;
};
