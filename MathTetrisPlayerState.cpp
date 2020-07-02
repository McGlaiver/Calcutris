// Fill out your copyright notice in the Description page of Project Settings.


#include "MathTetrisPlayerState.h"



void AMathTetrisPlayerState::AddScore(int a)
{

	if (streak >= streakDefAmount) {
		a = UseStreak(a);
		streak = 0;
	}

	playerScore += 10 * a;
	AdvanceLevel();

}


void AMathTetrisPlayerState::AdvanceLevel()
{

	correctEquations += 1;

	if (correctEquations / levelUpAmount >= playerLevel) {

		IncreaseSpeed();
		playerLevel++;

	}

}

int AMathTetrisPlayerState::UseStreak(int a)
{

	return a*2;

}




void AMathTetrisPlayerState::IncreaseSpeed()
{

	fallingSpeed += speedIncrement;

}




