// Capulet2kx : Advent of Code 2021

#include "CrabAlignmentProgram.h"

void UCrabAlignmentProgram::ProgramSetup()
{
	Super::ProgramSetup();
	ProgramName = FText::FromString(TEXT("Crab Alignment"));
	MessageString = TEXT("System online.");
	RefreshConsole();
}

void UCrabAlignmentProgram::RefreshConsole()
{
	ConsoleStrings.Reset();

	FString TitleString = TEXT("Crab Alignment : V");
	TitleString.AppendInt(SoftwareVersion);

	ConsoleStrings.Emplace(TitleString);

	ConsoleStrings.Emplace(TEXT("============"));
	ConsoleStrings.Emplace(MessageString);
	Super::RefreshConsole();
}

void UCrabAlignmentProgram::CalculateResult()
{
	LeastFuelPosition = -1;
	LeastFuelAmount = -1;
	int CheapestAmount = 0;
	int CheapestPosition = 0;
	
	TArray<int> UsedPositions;
	CrabPositions.GetKeys(UsedPositions);
	int MinCoord = UsedPositions[0];
	int MaxCoord = UsedPositions[0];

	for(int Position : UsedPositions)
	{
		if (Position > MaxCoord)
		{
			MaxCoord = Position;
		}
		else if (Position < MinCoord)
		{
			MinCoord = Position;
		}
	}

	for (int i = MinCoord; i <= MaxCoord; i++)
	{
		int FuelUsed = 0;
		for (int Position : UsedPositions)
		{
			int Distance = FMath::Abs(Position - i);
			int FuelCost = Distance;
			// Model with improved knowledge of cRaB eNgInEeRiNg
			if (SoftwareVersion == 2)
			{
				for (int j = 1; j < Distance; j++)
				{
					FuelCost += j;
				}
			}
			FuelUsed += FuelCost * CrabPositions[Position];
		}
		if (CheapestAmount == 0 || FuelUsed < CheapestAmount)
		{
			CheapestAmount = FuelUsed;
			CheapestPosition = i;
		}
	}

	LeastFuelAmount = CheapestAmount;
	LeastFuelPosition = CheapestPosition;
}

void UCrabAlignmentProgram::TestKeyPressed()
{
	if (!LoadDataFromFile(TEXT("CrabPositionTestData.txt")))
	{
		MessageString = TEXT("Error!<br>Couldn't load data.<br>Test failed.");
		RefreshConsole();
		return;
	}

	MessageString = TEXT("Loaded test data.<br>");

	CalculateResult();

	if (SoftwareVersion == 1)
	{
		if (LeastFuelPosition == 2 && LeastFuelAmount == 37)
		{
			UE_LOG(LogTemp, Warning, TEXT("2 tests passed.<br>Test Success."));
			MessageString.Append(TEXT("2 tests passed.<br>Test Success."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error! Unexpected values %i %i. Test failed."), LeastFuelPosition, LeastFuelAmount);
			MessageString.Append(TEXT("Error!<br>Unexpected value.<br>Test failed."));
		}
	}
	else
	{
		if (LeastFuelPosition == 5 && LeastFuelAmount == 168)
		{
			UE_LOG(LogTemp, Warning, TEXT("2 tests passed.<br>Test Success."));
			MessageString.Append(TEXT("2 tests passed.<br>Test Success."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error! Unexpected values %i %i. Test failed."), LeastFuelPosition, LeastFuelAmount);
			MessageString.Append(TEXT("Error!<br>Unexpected value.<br>Test failed."));
		}
	}
	RefreshConsole();
}

void UCrabAlignmentProgram::OperateKeyPressed()
{
	if (!LoadDataFromFile(TEXT("CrabPositionData.txt")))
	{
		MessageString = TEXT("Error!<br>Couldn't load data.<br>Test failed.");
		RefreshConsole();
		return;
	}

	MessageString = TEXT("Loaded test data.<br>");

	CalculateResult();

	MessageString.Append(TEXT("Position: "));
	MessageString.AppendInt(LeastFuelPosition);
	MessageString.Append(TEXT("<br>Fuel used:<br>"));
	MessageString.AppendInt(LeastFuelAmount);
	MessageString.Append(TEXT("<br>Analysis complete."));

	UE_LOG(LogTemp, Warning, TEXT("Fuel: %i, Position: %i"), LeastFuelAmount, LeastFuelPosition);
	RefreshConsole();

}

bool UCrabAlignmentProgram::LoadDataFromFile(const FString& FileName)
{
	TArray<FString> DataStrings;
	if (!LoadStringsFromFile(DataStrings, FileName))
	{
		MessageString = TEXT("Error!<br>Couldn't load strings from file.<br>Analysis failed.");
		RefreshConsole();
		return false;
	}

	CrabPositions.Reset();
	for (FString FoundString : DataStrings)
	{
		TArray<FString> CrabPositionStrings;
		DataStrings[0].ParseIntoArray(CrabPositionStrings, TEXT(","), true);
		for (FString FoundPositionString : CrabPositionStrings)
		{
			int FoundPosition = FCString::Atoi(*FoundPositionString);
			if (!CrabPositions.Contains(FoundPosition))
			{
				CrabPositions.Add(FoundPosition);
			}
			CrabPositions[FoundPosition]++;
		}
	}
	return true;
}


void UCrabAlignmentProgram::LeftKeyPressed()
{
	if (SoftwareVersion > 1)
	{
		SoftwareVersion--;
		MessageString = TEXT("V1 software<br>selected.");
		RefreshConsole();
	}
}

void UCrabAlignmentProgram::RightKeyPressed()
{
	if (SoftwareVersion < 2)
	{
		SoftwareVersion++;
		MessageString = TEXT("V2 software<br>selected.");
		RefreshConsole();
	}
}