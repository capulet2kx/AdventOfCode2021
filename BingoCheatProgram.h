// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComputerProgram.h"
#include "BingoCard.h"
#include "BingoCheatProgram.generated.h"

/**
  * Finds the first or last winning bingo cards, 
  * depending on the mode.
 *  Day 4 - Advent of Code 2021, Parts 1 & 2.
 */

UCLASS(Blueprintable)
class AOC2021_API UBingoCheatProgram : public UComputerProgram
{
	GENERATED_BODY()
	
public:
	virtual void ProgramSetup() override;
	virtual void RefreshConsole() override;
	
	virtual void OperateKeyPressed() override;
	virtual void TestKeyPressed() override;
	virtual void LeftKeyPressed() override;
	virtual void RightKeyPressed() override;

private:
	bool LoadDataFromFile(const FString& FileName);
	
	void StartGame();
	void DoTurn();
	int PickNumber();
	void PopulateWinningCards(TArray<int>& WinningCards, int NewNumber);

	TMap<int, bool> DrawnNumbers;
	TArray<FBingoCard> BingoCards;
	
	int TurnIndex = 0;
	TArray<int> NumberSequence;

	bool GameEnded = true;

	int WinningCardIndex = -1;
	int WinningScore = -1;
	int LastNumber = -1;

	bool WinMode = false;

	int GetNotWonCardsCount();
};
