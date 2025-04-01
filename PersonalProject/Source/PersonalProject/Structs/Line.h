#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Line.generated.h"

USTRUCT(BlueprintType)
struct PERSONALPROJECT_API FLine : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FLine()
		: Start(FVector::ZeroVector),
		  End(FVector::ZeroVector)
	{}

	FLine(FVector2D InStart, FVector2D InEnd) : Start(InStart), End(InEnd) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector2D Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector2D End;
};
