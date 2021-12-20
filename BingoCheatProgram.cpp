// Fill out your copyright notice in the Description page of Project Settings.


#include "BingoCheatProgram.h"

void UBingoCheatProgram::ProgramSetup()
{
	Super::ProgramSetup();
	ProgramName = FText::FromString(TEXT("Bingo Cheat"));
	MessageString = TEXT("System online.");
	RefreshConsole();
}

void UBingoCheatProgram::RefreshConsole()
{
	ConsoleStrings.Reset();
	if (WinMode)
	{
		ConsoleStrings.Emplace(TEXT("Bingo Cheat : Win"));
	}
	else
	{
		ConsoleStrings.Emplace(TEXT("Bingo Cheat : Lose"));
	}
	ConsoleStrings.Emplace(TEXT("============"));
	ConsoleStrings.Emplace(MessageString);
	Super::RefreshConsole();
}

void UBingoCheatProgram::StartGame()
{
	// Reset all drawn numbers to false
	DrawnNumbers.Reset();
	for (int i = 0; i < 100; i++)
	{
		DrawnNumbers.Emplace(i, false);
	}
	// Mark the game as in progress
	GameEnded = false;

	// Count the numbers that will be drawn
	int NumberSequenceLength = NumberSequence.Num();
	// For each number thaqt will be drawn
	for (TurnIndex = 0; TurnIndex < NumberSequenceLength && !GameEnded; TurnIndex++ )
	{
		DoTurn();
	}

	if (!WinMode)
	{
		WinningScore = BingoCards[WinningCardIndex].GetScore(DrawnNumbers, LastNumber);
		GameEnded = true;
		UE_LOG(LogTemp, Warning, TEXT("GAME ENDED : Final card index : %i"), WinningCardIndex);
		UE_LOG(LogTemp, Warning, TEXT("GAME ENDED : Final card score : %i"), WinningScore);
	}
	else
	{
		if (TurnIndex == NumberSequenceLength && !GameEnded)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ran out of numbers to draw, but no winner found!"));
		}
	}
}

void UBingoCheatProgram::DoTurn()
{
	int NewNumber = PickNumber();
	LastNumber = NewNumber;
	TArray<int> FoundWinningCards;
	PopulateWinningCards(FoundWinningCards, NewNumber);
	if (FoundWinningCards.Num() > 0)
	{
		if (WinMode)
		{
			WinningCardIndex = FoundWinningCards[0];
			WinningScore = BingoCards[WinningCardIndex].GetScore(DrawnNumbers, NewNumber);
			GameEnded = true;
			UE_LOG(LogTemp, Warning, TEXT("GAME ENDED : Final card index : %i"), WinningCardIndex);
			UE_LOG(LogTemp, Warning, TEXT("GAME ENDED : Final card score : %i"), WinningScore);
		}
		else
		{
			WinningCardIndex = FoundWinningCards[0];
			if (GetNotWonCardsCount() == 0)
			{
				GameEnded = true;
			}
		}
	}
}

int UBingoCheatProgram::PickNumber()
{
	int NewNumber = NumberSequence[TurnIndex];
	if (DrawnNumbers[NewNumber] == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error! Number was already drawn! %i"), NewNumber);
		return 0;
	}
	
	DrawnNumbers[NewNumber] = true;
	return NewNumber;
}

void UBingoCheatProgram::PopulateWinningCards(TArray<int>& WinningCards, int NewNumber)
{
	for (int i = 0; i < BingoCards.Num(); i++)
	{
		FBingoCard& BingoCard = BingoCards[i];
		if (BingoCard.CheckWin(NewNumber, DrawnNumbers))
		{
			WinningCards.Add(i);
		}
	}
}


bool UBingoCheatProgram::LoadDataFromFile(const FString& FileName)
{
	TArray<FString> DataStrings;
	if (!LoadStringsFromFile(DataStrings, FileName))
	{
		MessageString = TEXT("Error!<br>Couldn't load strings from file.<br>Analysis failed.");
		RefreshConsole();
		return false;
	}
	if (DataStrings.Num() < 7)
	{
		MessageString = TEXT("Error!<br>Insufficient data.<br>Analysis failed.");
		RefreshConsole();
		return false;
	}

	// Load the sequence in which numbers will be drawn
	TArray<FString> NumbersSequenceStrings;
	DataStrings[0].ParseIntoArray(NumbersSequenceStrings, TEXT(","), true);
	NumberSequence.Reset();
	for (FString FoundString : NumbersSequenceStrings)
	{
		NumberSequence.Add(FCString::Atoi(*FoundString));
	}

	// Now build the bingo cards
	bool IsBuildingCard = false;
	FBingoCard BingoCard;
	FWinLinesArray NewWinLines;

	// For every line of text
	for (int i = 1; i < DataStrings.Num(); i++)
	{
		// If it's an empy line
		if (DataStrings[i].IsEmpty())
		{
			// If we were building a card, an empty line means it is finished
			if (IsBuildingCard)
			{
				// Finalize card
				BingoCard.AddWinLineRowsAndCalculateColumns(NewWinLines);
				BingoCards.Add(BingoCard);
				IsBuildingCard = false;
			}
			continue;
		}
		// Else there is data in the line
		else
		{
			// If we're not building a card, start building one
			if (!IsBuildingCard)
			{
				IsBuildingCard = true;
				NewWinLines.WinLines.Reset();
				BingoCard.Reset();
			}

			// Convert the line of data to numbers and add it as a win line.
			FWinLine NewWinLine;
			TArray<FString> WinLineStrings;
			DataStrings[i].ParseIntoArray(WinLineStrings, TEXT(" "), true);
			for (FString FoundString : WinLineStrings)
			{
				NewWinLine.WinNumbers.Add(FCString::Atoi(*FoundString));
			}
			NewWinLines.WinLines.Add(NewWinLine);
		}
	}

	// We have now run out of lines of text.
	// If we were building a card, finalize it.
	if (IsBuildingCard)
	{
		// Finalize card
		BingoCard.AddWinLineRowsAndCalculateColumns(NewWinLines);
		BingoCards.Add(BingoCard);
		IsBuildingCard = false;
	}
	return true;
}


void UBingoCheatProgram::TestKeyPressed()
{
	if (!LoadDataFromFile(TEXT("BingoCheatTestData.txt")))
	{
		MessageString = TEXT("Error!<br>Couldn't load data.<br>Test failed.");
		RefreshConsole();
		return;
	}

	MessageString = TEXT("Loaded test data.<br>");

	StartGame();
	if (WinMode)
	{
		if (WinningScore == 4512)
		{
			UE_LOG(LogTemp, Warning, TEXT("Win mode : Test Passed."));
			MessageString.Append(TEXT("1 test passed.<br>Test success."));
			RefreshConsole();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Win mode : Test failed."));
			MessageString.Append(TEXT("1 test failed.<br>Test failed!."));
			RefreshConsole();
		}
	}
	else
	{
		if (WinningScore == 1924)
		{
			UE_LOG(LogTemp, Warning, TEXT("Lose mode : Test Passed."));
			MessageString.Append(TEXT("1 test passed.<br>Test success."));
			RefreshConsole();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Lose mode : Test Failed."));
			MessageString.Append(TEXT("1 test failed.<br>Test failed!."));
			RefreshConsole();
		}
	}
	
}

void UBingoCheatProgram::OperateKeyPressed()
{
	if (!LoadDataFromFile(TEXT("BingoCheatData.txt")))
	{
		MessageString = TEXT("Error!<br>Couldn't load data.<br>Analasys failed.");
		RefreshConsole();
		return;
	}
	MessageString = TEXT("Loaded diagnostics data.<br>");
	
	StartGame();

	MessageString.Append(TEXT("Final score:<br>"));
	MessageString.AppendInt(WinningScore);

	UE_LOG(LogTemp,Warning,TEXT("Final score: %i"), WinningScore)
	RefreshConsole();
}

void UBingoCheatProgram::LeftKeyPressed()
{
	if (WinMode)
	{
		WinMode = false;
		ClearMessageString();
		RefreshConsole();
	}
}

void UBingoCheatProgram::RightKeyPressed()
{
	if (!WinMode)
	{
		WinMode = true;
		ClearMessageString();
		RefreshConsole();
	}
}

int UBingoCheatProgram::GetNotWonCardsCount()
{
	int NotWonCardsCount = 0;
	for (int i = 0; i < BingoCards.Num(); i++)
	{
		if (!BingoCards[i].GetCardWon())
		{
			NotWonCardsCount++;
		}
	}
	return NotWonCardsCount;
}