#include "KeyPad.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "../PrimarySystems/PrimaryPlayerCharacter.h"
#include "../PrimarySystems/PrimaryPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "../UI/KeyPadUI.h"

AKeyPad::AKeyPad()
{
	KeyPadMesh = CreateDefaultSubobject<UStaticMeshComponent>("Log");
	RootComponent = KeyPadMesh;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	BoxComponent->SetupAttachment(KeyPadMesh);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKeyPad::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AKeyPad::OnBoxEndOverlap);

	KeyPrompt = CreateDefaultSubobject<UWidgetComponent>("KeyPrompt");
	KeyPrompt->SetupAttachment(KeyPadMesh);

	KeyPadUI = CreateDefaultSubobject<UWidgetComponent>("KeyPadUI");
	KeyPadUI->SetupAttachment(KeyPadMesh);
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(KeyPadMesh);

	PrimaryActorTick.bCanEverTick = true;
}

void AKeyPad::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APrimaryPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerController = Cast<APrimaryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (Player)
	{
		Player->OnInteract.AddDynamic(this, &AKeyPad::HandleKeyPadInteract);
		Player->OnCancel.AddDynamic(this, &AKeyPad::HandleStopViewKeyPad);
	}

	if (UKeyPadUI* UI = Cast<UKeyPadUI>(KeyPadUI->GetWidget()))
	{
		UI->DoorToOpen = DoorToOpen;
	}
}

void AKeyPad::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APrimaryPlayerCharacter>(OtherActor))
	{
		bPlayerInRange = true;
	}
}

void AKeyPad::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APrimaryPlayerCharacter>(OtherActor))
	{
		bPlayerInRange = false;
		KeyPrompt->SetVisibility(false);
		bInteractable = false;
		if (KeyPadUI->IsVisible())
		{
			KeyPadUI->SetVisibility(false);
		}
	}
}

void AKeyPad::HandleKeyPadInteract()
{
	if (bPlayerInRange && bCanInteract)
	{
		if (!bInteractingWithKeyPad)
		{
			KeyPrompt->SetVisibility(false);
			bInteractable = false;
			KeyPadUI->SetVisibility(true);
			PlayerController->EnableMouseInGame();
			bInteractingWithKeyPad = true;
			bCanChangeViewTarget = true;
		}
	}
}

void AKeyPad::HandleStopViewKeyPad()
{
	if (bPlayerInRange && bCanInteract)
	{
		if (bInteractingWithKeyPad)
		{
			KeyPrompt->SetVisibility(true);
			bInteractable = true;
			KeyPadUI->SetVisibility(false);
			PlayerController->DisableMouse();
			bInteractingWithKeyPad = false;
			bCanChangeViewTargetToPlayer = true;
		}
	}
}

void AKeyPad::ChangeViewTarget(float DeltaTime, AActor* Target, bool& CanChangeViewFlag)
{
	const float TimeBetweenCameraChange = 2.0f;
	const float SmoothBlendTime = 0.75f;
	TimeToNextCameraChange -= DeltaTime;

	if (TimeToNextCameraChange <= 0.0f)
	{
		if (PlayerController)
		{
			if (PlayerController->GetViewTarget() != Target)
			{
				PlayerController->SetViewTargetWithBlend(Target, SmoothBlendTime);
				CanChangeViewFlag = false;
			}
			else
			{
				CanChangeViewFlag = false;

			}
		}
	}
}

void AKeyPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanInteract)
	{
		if (!bInteractingWithKeyPad)
		{
			KeyPrompt->SetVisibility(true);
			bInteractable = true;
		}
	}
	else
	{
		KeyPrompt->SetVisibility(false);
		bInteractable = false;
		if (KeyPadUI->IsVisible())
		{
			KeyPadUI->SetVisibility(false);
		}
	}

	if (bCanChangeViewTarget)
	{
		ChangeViewTarget(DeltaTime, this, bCanChangeViewTarget);
	}
	if (bCanChangeViewTargetToPlayer)
	{
		ChangeViewTarget(DeltaTime, Player, bCanChangeViewTargetToPlayer);
	}

	bCanInteract = false;
}

