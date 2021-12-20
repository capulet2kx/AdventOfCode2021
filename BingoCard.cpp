// Fill out your copyright notice in the Description page of Project Settings.


#include "BingoCard.h"

bool FWinLine::CheckWin(const TMap<int, bool>& DrawnNumbers)
{
	for (int FoundInt : WinNumbers)
	{
		if (DrawnNumbers[FoundInt] == false)
		{
			return false;
		}
	}
	return true;
}

bool FBingoCard::CheckWin(int NewNumber, const TMap<int, bool>& DrawnNumbers)
{
	if (CardWon)
	{
		return false;
	}

	// Get the win lines array for the new number
	FWinLinesArray FoundWinLinesArray = WinLinesByNumber[NewNumber];
	for (FWinLine WinLine : FoundWinLinesArray.WinLines)
	{
		if (WinLine.CheckWin(DrawnNumbers))
		{
			CardWon = true;
			return true;
		}
	}
	return false;
}

FBingoCard::FBingoCard()
{
	Reset();
}

void FBingoCard::Reset()
{
	CardNumbers.Reset();
	WinLinesByNumber.Reset();
	SetUpWinLines();
}

int FBingoCard::GetScore(const TMap<int, bool>& DrawnNumbers, int LastNumber)
{
	int Score = 0;
	for (int FoundInt : CardNumbers)
	{
		if (!DrawnNumbers[FoundInt])
		{
			Score += FoundInt;
		}
	}
	Score *= LastNumber;
	return Score;
}

bool FBingoCard::GetCardWon()
{
	return CardWon;
}


void FBingoCard::SetUpWinLines()
{
	WinLinesByNumber.Reset();
	// Create an entry for each number that can be drawn
	for (int i = 0; i <= 99; i++)
	{
		FWinLinesArray NewWinLinesArray;
		WinLinesByNumber.Emplace(i, NewWinLinesArray);
	}
}


void FBingoCard::AddWinLineRowsAndCalculateColumns(const FWinLinesArray& NewWinLines)
{
	// Add the rows
	for (FWinLine FoundWinLine : NewWinLines.WinLines)
	{
		for (int FoundInt : FoundWinLine.WinNumbers)
		{
			WinLinesByNumber.FindOrAdd(FoundInt).WinLines.Add(FoundWinLine);
			CardNumbers.Add(FoundInt);
		}
	}

	// Count how many columns there are
	int ColumnCount = NewWinLines.WinLines[0].WinNumbers.Num();

	// Add the columns, we need to construct these winlines from the rows.
	for (int i = 0; i < ColumnCount; i++)
	{
		FWinLine NewWinLine;
		for (FWinLine FoundWinLine : NewWinLines.WinLines)
		{
			NewWinLine.WinNumbers.Add(FoundWinLine.WinNumbers[i]);
		}
		for (int FoundInt : NewWinLine.WinNumbers)
		{

			if (WinLinesByNumber.Contains(FoundInt))
			{
				WinLinesByNumber[FoundInt].WinLines.Add(NewWinLine);
			}
		}
	}
}