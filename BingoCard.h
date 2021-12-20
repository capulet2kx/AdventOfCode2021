// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BingoCard.generated.h"

// A series of numbers and a function to check if they have 
// all been drawn, winning the game.
USTRUCT()
struct FWinLine
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TArray<int> WinNumbers;

	bool CheckWin(const TMap<int, bool>& DrawnNumbers);
};

// An array of WinLines, housed in a struct, representing all the
// winning lines on a particular bingo card.
USTRUCT()
struct FWinLinesArray
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TArray<FWinLine> WinLines;
};

// A bingo card.
USTRUCT()
struct FBingoCard
{
	GENERATED_BODY()

public:
	FBingoCard();
	bool CheckWin(const int NewNumber, const TMap<int, bool>& DrawnNumbers);
	int GetScore(const TMap<int, bool>& DrawnNumbers, const int LastNumber);
	void Reset();
	// Add an array of winlines that are the rows, columns will be calculated
	void AddWinLineRowsAndCalculateColumns(const FWinLinesArray& NewWinLines);
	bool GetCardWon();
private:
	// A map of each number that can be drawn, to the win lines that contain it.
	// Allows us to check only the win lines for the most recently drawn number.
	UPROPERTY()
		TMap<int, FWinLinesArray> WinLinesByNumber;

	// A list of all the numbers that appear on this card, used for scoring later.
	TArray<int> CardNumbers;
	// Whether this card has already won, so it doesn't register as winning more than once.
	bool CardWon = false;

	// Set up WinLinesByNumber to have an FWinLinesArray ready for each 
	// number that can be drawn
	void SetUpWinLines();
};
