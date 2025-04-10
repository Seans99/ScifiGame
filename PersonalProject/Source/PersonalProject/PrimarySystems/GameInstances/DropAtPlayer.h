#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PersonalProject/Structs/ItemStruct.h"
#include "DropAtPlayer.generated.h"


UCLASS()
class PERSONALPROJECT_API UDropAtPlayer : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Drop(UObject* ItemToDrop, FItemData* ItemData);
};
