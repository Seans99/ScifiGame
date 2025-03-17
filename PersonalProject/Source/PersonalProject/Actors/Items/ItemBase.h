#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PersonalProject/Structs/ItemStruct.h"
#include "ItemBase.generated.h"

UCLASS()
class PERSONALPROJECT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;

};
