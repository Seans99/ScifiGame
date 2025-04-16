#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tablet_Log.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class UWidgetComponent;
class UBoxComponent;

class APrimaryPlayerCharacter;

UCLASS()
class PERSONALPROJECT_API ATablet_Log : public AActor
{
	GENERATED_BODY()
	
public:	
	ATablet_Log();

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LogMesh;

	UPROPERTY(EditAnywhere)
	UPointLightComponent* PointLight;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* KeyPrompt;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* Log;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

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
	void HandleInteract();

	UFUNCTION()
	void HandleClose();

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool bCanInteract = false;
	bool bInteracting = false;
	bool bPlayerInRange = false;

private:
	APrimaryPlayerCharacter* Player;
};
