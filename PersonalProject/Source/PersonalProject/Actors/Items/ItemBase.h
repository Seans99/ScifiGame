#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PersonalProject/Structs/ItemStruct.h"
#include "ItemBase.generated.h"

class APrimaryPlayerCharacter;

class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;

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
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* KeyPrompt;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;

public:
	FItemData GetItemData();

	UFUNCTION()
	void PickupItem();

	UClass* GetItemClass() const;

public:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	APrimaryPlayerCharacter* Player;

	bool bPlayerInRange = false;
};
