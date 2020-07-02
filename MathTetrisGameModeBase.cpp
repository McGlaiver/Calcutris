// Copyright Epic Games, Inc. All Rights Reserved.


#include "MathTetrisGameModeBase.h"

AMathTetrisGameModeBase::AMathTetrisGameModeBase() {

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

}

void AMathTetrisGameModeBase::StartGame()
{

	newGame = false;
	if (newGameUi) {
		newGameUi->RemoveFromViewport();
	}
	if (gameOverUi) {
		FGenericPlatformMisc::RequestExit(false);
	}
	else {

		myUI = CreateWidget(GetWorld(), myUIClass);
		myUI->AddToViewport();

		SpawnNewNumber();
	}

}

void AMathTetrisGameModeBase::GameOver()
{

	newGame = true;
	gameOverUi = CreateWidget(GetWorld(), gameOverClass);
	gameOverUi->AddToViewport();
	Super::EndPlay(EEndPlayReason::LevelTransition);

}

void AMathTetrisGameModeBase::SpawnNewNumber()
{

	FVector SpawnPosition = FVector(0.0f, 0.0f, 200.0f);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(newLetter, &SpawnPosition, &SpawnRotation);


	numberIsActive = true;

}

void AMathTetrisGameModeBase::BeginPlay()
{
	
	Super::BeginPlay();
	
	
	if ((newGameUiClass != nullptr) && (newGame == true)) {

		newGameUi = CreateWidget(GetWorld(), newGameUiClass);
		newGameUi->AddToViewport();


	}

}

void AMathTetrisGameModeBase::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	if (numberIsActive == false) {
		SpawnNewNumber();
	}

}
