#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GUIBase.generated.h"

class APrimaryGameMode;

UCLASS(Abstract)
class PERSONALPROJECT_API UGUIBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	APrimaryGameMode* GameMode;
};