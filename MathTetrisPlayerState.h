// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Blueprint/UserWidget.h"
#include "MathTetrisPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MATHTETRIS_API AMathTetrisPlayerState : public APlayerState
{
	GENERATED_BODY()



public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int playerScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int neededTotal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int playerLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int correctEquations = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int previousNumber;

	UPROPERTY(EditAnywhere)
		float fallingSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
		float speedIncrement = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int levelUpAmount = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString lastEquation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int streak = 0;

	UPROPERTY(EditAnywhere)
		int streakDefAmount = 5;


	

	
	void IncreaseSpeed();

	void AddScore(int a);

	void AdvanceLevel();

	int UseStreak(int a);


	

};
