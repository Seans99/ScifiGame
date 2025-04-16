#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PrimaryPlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJournalSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventorySignature);

class UCameraComponent;
class APrimaryGameMode;
class APrimaryPlayerController;

UCLASS()
class PERSONALPROJECT_API APrimaryPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcess = "true"))
	UCameraComponent* Camera;

public:
	// Sets default values for this character's properties
	APrimaryPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* CancelAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* JournalAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* InventoryAction;

protected:
	void Move(const FInputActionValue& InputValue);
	void Jump();
	void Look(const FInputActionValue& InputValue);
	void Attack();
	void StartSprint();
	void StopSprint();
	void Interact();
	void Cancel();
	void Journal();
	void Inventory();

	UFUNCTION()
	void HandleStaminaDepleted();

public:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 600;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 1200;

	bool bCanInteract = false;

public:
	UPROPERTY()
	FInteractSignature OnInteract;

	UPROPERTY()
	FInteractSignature OnCancel;

	UPROPERTY()
	FInventorySignature OnInventory;

public:
	FVector ObjectivePoint;
	int32 CurrentObjectiveID;

	UPROPERTY()
	FJournalSignature OnShowJournal;

private:
	APrimaryGameMode* GameMode;

	bool bIsMoving = false;
	void StoppedMoving();

	FVector StartTrace;
	FVector EndTrace;
};
