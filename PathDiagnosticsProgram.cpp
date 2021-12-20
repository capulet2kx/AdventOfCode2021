// Fill out your copyright notice in the Description page of Project Settings.


#include "PathDiagnosticsProgram.h"
#include "Containers/Map.h"

void UPathDiagnosticsProgram::ProgramSetup()
{
	Super::ProgramSetup();
	ProgramName = FText::FromString(TEXT("Path Diagnostics"));
	MessageString = TEXT("System online.");
	RefreshConsole();
}

void UPathDiagnosticsProgram::RefreshConsole()
{
	ConsoleStrings.Reset();
	FString NameString = ProgramName.ToString();
	if (SoftwareVersion == 1)
	{
		NameString.Append(TEXT(" V1"));
	}
	else if (SoftwareVersion == 2)
	{
		NameString.Append(TEXT(" V2"));
	}
	ConsoleStrings.Emplace(NameString);

	ConsoleStrings.Emplace(TEXT("============"));

	ConsoleStrings.Emplace(MessageString);

	Super::RefreshConsole();
}

void UPathDiagnosticsProgram::TestKeyPressed()
{
	if (!LoadDataFromFile(TEXT("PlannedPathTestData.txt")))
	{
		MessageString = TEXT("Error!<br>Couldn't load data.<br>Test failed.");
		RefreshConsole();
		return;
	}
	MessageString = TEXT("Test data loaded okay.<br>");

	if (SoftwareVersion == 1)
	{
		MessageString.Append(TEXT("Testing V1 Calculations.<br>"));
		CalculateResultsV1();
	}
	else if (SoftwareVersion == 2)
	{
		MessageString.Append(TEXT("Testing V2 Calculations.<br>"));
		CalculateResultsV2();
	}
	else
	{
		MessageString.Append(TEXT("Error!<br>Invalid software<br>version.<br>Test failed."));
		RefreshConsole();
		return;
	}

	if (SoftwareVersion == 1)
	{
		if (FinalDepth == 10 && FinalHorizontalDistance == 15 && MultipliedFinalValues == 150)
		{
			MessageString.Append(TEXT("3 Tests passed okay.<br>V1 Test success."));
			UE_LOG(LogTemp, Warning, TEXT("3 Tests passed okay. V1 Test success."));
		}
		else
		{
			MessageString.Append(TEXT("V1 test failed.<br>Unexpected values(s).<br>Test failure."));
			UE_LOG(LogTemp, Warning, TEXT("V1 test failed: Final Depth: %i, Dist: %i, Multiplied: %i"), FinalDepth, FinalHorizontalDistance, MultipliedFinalValues);
		}
	}
	else
	{
		if (FinalDepth == 60 && FinalHorizontalDistance == 15 && MultipliedFinalValues == 900)
		{
			MessageString.Append(TEXT("3 Tests passed okay.<br>V2 Test success."));
			UE_LOG(LogTemp, Warning, TEXT("3 Tests passed okay. V2 Test success."));
		}
		else
		{
			MessageString.Append(TEXT("V2 test failed.<br>Unexpected values(s).<br>Test failure."));
			UE_LOG(LogTemp, Warning, TEXT("V2 test failed: Final Depth: %i, Dist: %i, Multiplied: %i"), FinalDepth, FinalHorizontalDistance, MultipliedFinalValues);
		}
	}
	RefreshConsole();
}

void UPathDiagnosticsProgram::OperateKeyPressed()
{
	if (!LoadDataFromFile(TEXT("PlannedPathData.txt")))
	{
		MessageString = TEXT("Error!<br>Couldn't load data.<br>Analasys failed.");
		RefreshConsole();
		return;
	}

	if (SoftwareVersion == 1)
	{
		CalculateResultsV1();
	}
	else if (SoftwareVersion == 2)
	{
		CalculateResultsV2();
	}
	else
	{
		MessageString = TEXT("Error!<br>Invalid software<br>version.<br>Analysis failed.");
		RefreshConsole();
		return;
	}

	MessageString = TEXT("Calculating destination.<br>");
	
	MessageString.Append(TEXT("Depth: "));
	MessageString.AppendInt(FinalDepth);
	MessageString.Append(TEXT("<br>"));
	UE_LOG(LogTemp, Warning, TEXT("Final Depth: %i"), FinalDepth);

	MessageString.Append(TEXT("Horizontal dist: "));
	MessageString.AppendInt(FinalHorizontalDistance);
	MessageString.Append(TEXT("<br>"));
	UE_LOG(LogTemp, Warning, TEXT("Final Horizontal Distance: %i"), FinalHorizontalDistance);
	
	MessageString.Append(TEXT(" <br>"));
	MessageString.Append(TEXT("Checksum: "));
	MessageString.AppendInt(MultipliedFinalValues);
	UE_LOG(LogTemp, Warning, TEXT("Final values multiplied: %i"), MultipliedFinalValues);

	RefreshConsole();
}

void UPathDiagnosticsProgram::LeftKeyPressed()
{
	if (SoftwareVersion > 1)
	{
		SoftwareVersion--;
		ClearMessageString();
		RefreshConsole();
	}
}

void UPathDiagnosticsProgram::RightKeyPressed()
{
	if (SoftwareVersion < 2)
	{
		SoftwareVersion++;
		ClearMessageString();
		RefreshConsole();
	}
}

bool UPathDiagnosticsProgram::LoadDataFromFile(const FString& FileName)
{
	PathData.Reset();
	TArray<FString> ScanDataStrings;
	if (!LoadStringsFromFile(ScanDataStrings, FileName))
	{
		MessageString = TEXT("Error!<br>Couldn't load strings from file.<br>Analysis failed.");
		RefreshConsole();
		return false;
	}

	for (FString FoundString : ScanDataStrings)
	{
		FString LeftSide, RightSide;
		FoundString.Split(TEXT(" "), &LeftSide, &RightSide);
		TPair<FString,int> NewPair(LeftSide, FCString::Atoi(*RightSide));
		PathData.Emplace(NewPair);
	}
	return true;
}

void UPathDiagnosticsProgram::CalculateResultsV1()
{
	int UpTotal = 0;
	int DownTotal = 0;
	int ForwardTotal = 0;
	
	for (auto Pair : PathData)
	{
		if (Pair.Key == TEXT("forward"))
		{
			ForwardTotal += Pair.Value;
		}
		if (Pair.Key == TEXT("down"))
		{
			DownTotal += Pair.Value;
		}
		if (Pair.Key == TEXT("up"))
		{
			UpTotal += Pair.Value;
		}
	}
	
	FinalDepth = DownTotal - UpTotal;
	FinalHorizontalDistance = ForwardTotal;
	MultipliedFinalValues = FinalDepth * FinalHorizontalDistance;
}

void UPathDiagnosticsProgram::CalculateResultsV2()
{
	FinalHorizontalDistance = 0;
	FinalDepth = 0;
	int Aim = 0;

	for (auto Pair : PathData)
	{
		if (Pair.Key == TEXT("forward"))
		{
			FinalHorizontalDistance += Pair.Value;
			FinalDepth += Aim * Pair.Value;
		}
		if (Pair.Key == TEXT("down"))
		{
			Aim += Pair.Value;
		}
		if (Pair.Key == TEXT("up"))
		{
			Aim -= Pair.Value;
		}
	}

	MultipliedFinalValues = FinalDepth * FinalHorizontalDistance;
}