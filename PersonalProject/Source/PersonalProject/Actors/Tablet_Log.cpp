#include "Tablet_Log.h"
#include "Components/PointLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "../PrimarySystems/PrimaryPlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

ATablet_Log::ATablet_Log()
{
	LogMesh = CreateDefaultSubobject<UStaticMeshComponent>("Log");
	RootComponent = LogMesh;

	PointLight = CreateDefaultSubobject<UPointLightComponent>("PointLight");
	PointLight->SetupAttachment(LogMesh);

	KeyPrompt = CreateDefaultSubobject<UWidgetComponent>("KeyPromptWidget");
	KeyPrompt->SetupAttachment(LogMesh);

	Log = CreateDefaultSubobject<UWidgetComponent>("LogUI");
	Log->SetupAttachment(LogMesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(LogMesh);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATablet_Log::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATablet_Log::OnBoxEndOverlap);

	PrimaryActorTick.bCanEverTick = true;
}

void ATablet_Log::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<APrimaryPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		Player->OnInteract.AddDynamic(this, &ATablet_Log::HandleInteract);
		Player->OnCancel.AddDynamic(this, &ATablet_Log::HandleClose);
	}
}

void ATablet_Log::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APrimaryPlayerCharacter>(OtherActor))
	{
		bPlayerInRange = true;
	}
}

void ATablet_Log::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APrimaryPlayerCharacter>(OtherActor))
	{
		bPlayerInRange = false;
		KeyPrompt->SetVisibility(false);
		if (Log->IsVisible())
		{
			Log->SetVisibility(false);
		}
	}
}

void ATablet_Log::HandleInteract()
{
	if (bPlayerInRange && bCanInteract)
	{
		if (!bInteracting)
		{
			bInteracting = true;
			KeyPrompt->SetVisibility(false);
			Log->SetVisibility(true);
		}
	}
}

void ATablet_Log::HandleClose()
{
	if (bPlayerInRange && bCanInteract)
	{
		if (bInteracting)
		{
			bInteracting = false;
			KeyPrompt->SetVisibility(true);
			Log->SetVisibility(false);
		}
	}
}

void ATablet_Log::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanInteract)
	{
		if (!bInteracting)
		{
			KeyPrompt->SetVisibility(true);
		}
	}
	else
	{
		KeyPrompt->SetVisibility(false);
		if (Log->IsVisible())
		{
			Log->SetVisibility(false);
		}
	}

	if (Log->IsVisible())
	{
		FVector Start = Log->GetComponentLocation();
		FVector Target = Player->GetActorLocation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
		
		Log->SetWorldRotation(LookAtRotation);
	}

	bCanInteract = false;
}

