#include "GUIBase.h"
#include "../PrimarySystems/PrimaryGameMode.h"
#include <Kismet/GameplayStatics.h>

void UGUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<APrimaryGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

