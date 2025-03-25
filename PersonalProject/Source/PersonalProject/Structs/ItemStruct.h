#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemStruct.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equipment UMETA(DisplayName = "Equipment"),
	Consumable UMETA(DisplayName = "Consumable"),
	Other UMETA(DisplayName = "Other"),
};

USTRUCT(BlueprintType)
struct PERSONALPROJECT_API FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FItemData()
		: bItemStackable(false),
		  ItemAmount(0),
		  ItemMesh(nullptr),
		  ItemImage(nullptr),
		  bItemHealAble(false),
		  ItemHealAmount(0)
	{}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bItemStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* ItemMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bItemHealAble;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int ItemHealAmount;
};
