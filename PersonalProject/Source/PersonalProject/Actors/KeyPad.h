#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyPad.generated.h"

class UWidgetComponent;
class UBoxComponent;
class UCameraComponent;

class APrimaryPlayerCharacter;
class APrimaryPlayerController;

UCLASS()
class PERSONALPROJECT_API AKeyPad : public AActor
{
	GENERATED_BODY()
	
public:
	AKeyPad();

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* KeyPadMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* KeyPadUI;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* KeyPrompt;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void HandleKeyPadInteract();

	UFUNCTION()
	void HandleStopViewKeyPad();

	UFUNCTION()
	void ChangeViewTarget(float DeltaTime, AActor* Target, bool& CanChangeViewFlag);

public:
	UPROPERTY(EditAnywhere)
	FString DoorToOpen;

public:
	bool bCanInteract = false;
	bool bInteractable = false;
	bool bCanChangeViewTarget = false;
	bool bCanChangeViewTargetToPlayer = false;
	bool bPlayerInRange = false;

private:
	float TimeToNextCameraChange;
	bool bInteractingWithKeyPad = false;

private:
	APrimaryPlayerCharacter* Player;
	APrimaryPlayerController* PlayerController;

public:	
	virtual void Tick(float DeltaTime) override;

};
