#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PrimaryGameMode.generated.h"

class UTitleScreen;
class UMainMenu;
class USettings;
class UPauseMenu;
class UPlayerHUD;
class UObjectiveUI;
class APrimaryPlayerController;
class UObjectiveManager;
class UInventoryUI;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	ETitleScreen		UMETA(DisplayName = "TitleScreen"),
	EMainMenu			UMETA(DisplayName = "MainMenu"),
	ESettings			UMETA(DisplayName = "Settings"),
	EInGame				UMETA(DisplayName = "In Game"),
	EPause				UMETA(DisplayName = "Pause"),
	EGameOver			UMETA(DisplayName = "Game Over")
};

UCLASS()
class PERSONALPROJECT_API APrimaryGameMode : public AGameModeBase
{
	GENERATED_BODY()

private: 
	APrimaryGameMode();

	EGameState CurrentState;

public:
	void SetState(EGameState NewState);

	EGameState GetCurrentState() const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public: 
	UPROPERTY(EditAnywhere, Category = "Level")
	FName MainLevel;

	UPROPERTY(EditAnywhere, Category = "Level")
	FName Level1;

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UTitleScreen> TitleScreenWidgetClass;

	UPROPERTY()
	UTitleScreen* TitleScreenWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMainMenu> MainMenuWidgetClass;

	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USettings> SettingsWidgetClass;

	UPROPERTY()
	USettings* SettingsWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPauseMenu> PauseMenuWidgetClass;

	UPROPERTY()
	UPauseMenu* PauseMenuWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHUD> PlayerHudWidgetClass;

	UPROPERTY()
	UPlayerHUD* PlayerHudWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UObjectiveUI> ObjectiveWidgetClass;

	UPROPERTY()
	UObjectiveUI* ObjectiveWidget;

public:
	bool bShowObjective = false;
	
public:
	UPROPERTY()
	APrimaryPlayerController* Controller;

	UPROPERTY()
	UObjectiveManager* ObjManager;

private:
	FTimerHandle TimerHandle;

private:
	void TitleScreenSetup();
	void MainMenuSetup();
	void SettingsSetup();
	void InGameSetup();
	void PauseSetup();
	void GameOverSetup();
	void Objective();
	void RemoveObjectiveWidget();
};
