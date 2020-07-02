// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MathTetrisGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MATHTETRIS_API AMathTetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


	
public:

	AMathTetrisGameModeBase();

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> newLetter;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> myUIClass;
	UUserWidget* myUI;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> newGameUiClass;
	UUserWidget* newGameUi;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> gameOverClass;
	UUserWidget* gameOverUi;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool numberIsActive = true;

	bool newGame = true;
	

	void StartGame();

	void GameOver();

	void SpawnNewNumber();


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
