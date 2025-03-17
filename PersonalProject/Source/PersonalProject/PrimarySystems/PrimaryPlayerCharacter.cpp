#include "PrimaryPlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../PrimarySystems/PrimaryGameMode.h"
#include "../PrimarySystems/PrimaryPlayerController.h"
#include "../Components/StaminaComponent.h"
#include "../Actors/KeyPad.h"
#include "../Actors/Tablet_Log.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APrimaryPlayerCharacter::APrimaryPlayerCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APrimaryPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = WalkSpeed;
	}

	if (UStaminaComponent* Stamina = FindComponentByClass<UStaminaComponent>())
	{
		Stamina->OnStaminaDepleted.AddDynamic(this, &APrimaryPlayerCharacter::HandleStaminaDepleted);
	}
}

// Called every frame
void APrimaryPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APrimaryPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrimaryPlayerCharacter::Move);
		Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &APrimaryPlayerCharacter::StoppedMoving);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APrimaryPlayerCharacter::Jump);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrimaryPlayerCharacter::Look);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APrimaryPlayerCharacter::Attack);
		Input->BindAction(SprintAction, ETriggerEvent::Started, this, &APrimaryPlayerCharacter::StartSprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &APrimaryPlayerCharacter::StopSprint);
		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APrimaryPlayerCharacter::Interact);
		Input->BindAction(CancelAction, ETriggerEvent::Triggered, this, &APrimaryPlayerCharacter::Cancel);
		Input->BindAction(JournalAction, ETriggerEvent::Triggered, this, &APrimaryPlayerCharacter::Journal);
		Input->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &APrimaryPlayerCharacter::Inventory);
	}
}

void APrimaryPlayerCharacter::Move(const FInputActionValue& InputValue)
{
	bIsMoving = true;
	FVector2D InputVector = InputValue.Get<FVector2D>();

	const FVector ForwardDirection = GetActorForwardVector();
	const FVector RightDirection = GetActorRightVector();

	AddMovementInput(ForwardDirection, InputVector.Y);
	AddMovementInput(RightDirection, InputVector.X);
}

void APrimaryPlayerCharacter::StoppedMoving()
{
	bIsMoving = false;
}

void APrimaryPlayerCharacter::Jump()
{
	ACharacter::Jump();
}

void APrimaryPlayerCharacter::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void APrimaryPlayerCharacter::Attack()
{
	
}

void APrimaryPlayerCharacter::StartSprint()
{
	if (bIsMoving)
	{
		if (UStaminaComponent* Stamina = FindComponentByClass<UStaminaComponent>())
		{
			if (Stamina->CurrentStamina > 0)
			{
				Stamina->LoseStamina();
				if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
				{
					MovementComponent->MaxWalkSpeed = SprintSpeed;
				}
			}
		}
	}
}

void APrimaryPlayerCharacter::StopSprint()
{
	if (UStaminaComponent* Stamina = FindComponentByClass<UStaminaComponent>())
	{
		Stamina->StopUsingStamina();
		Stamina->RegainStamina();
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void APrimaryPlayerCharacter::HandleStaminaDepleted()
{
	StopSprint();
}

void APrimaryPlayerCharacter::Interact()
{
	TArray<AActor*> FoundKeyPads;
	TArray<AActor*> FoundLogs;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeyPad::StaticClass(), FoundKeyPads);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATablet_Log::StaticClass(), FoundLogs);

	for (AActor* KeyPad : FoundKeyPads)
	{
		if (KeyPad)
		{
			if (AKeyPad* keypad = Cast<AKeyPad>(KeyPad))
			{
				if (keypad->bInteractable)
				{
					if (OnInteractKeyPad.IsBound()) OnInteractKeyPad.Broadcast();
				}
			}
		}
	}

	for (AActor* Log : FoundLogs)
	{
		if (Log)
		{
			if (ATablet_Log* log = Cast<ATablet_Log>(Log))
			{
				if (log->bInteractable)
				{
					if (OnInteractLog.IsBound()) OnInteractLog.Broadcast();
				}
			}
		}
	}
}

void APrimaryPlayerCharacter::Cancel()
{
	if (OnCancel.IsBound()) OnCancel.Broadcast();
}

void APrimaryPlayerCharacter::Journal()
{
	if (OnShowJournal.IsBound()) OnShowJournal.Broadcast();
}

void APrimaryPlayerCharacter::Inventory()
{
	// Open inventory
}



