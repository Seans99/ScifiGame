#pragma once

#include "CoreMinimal.h"
#include "GUIBase.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "InventoryUI.generated.h"

UCLASS()
class PERSONALPROJECT_API UInventoryUI : public UGUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void RefreshInventory();
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemDesc;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWrapBox* InventoryWrapBox;

	FText ItemNameText;
	FText ItemDescText;
};
