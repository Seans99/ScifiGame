#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct PERSONALPROJECT_API FTile : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FTile()
		: X(0),
		  Y(0)
	{}

	FTile(int InX, int InY) : X(InX), Y(InY) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int Y;
};
