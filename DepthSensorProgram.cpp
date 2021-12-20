// Capulet2kx : Advent of Code 2021

#include "DepthSensorProgram.h"

void UDepthSensorProgram::ProgramSetup()
{
	Super::ProgramSetup();
	ProgramName = FText::FromString(TEXT("Depth Sensor"));
	RefreshConsole();
}

void UDepthSensorProgram::RefreshConsole()
{
	ConsoleStrings.Reset();
	ConsoleStrings.Emplace(ProgramName.ToString());
	
	ConsoleStrings.Emplace(TEXT("============"));
	
	if (!HideSamplesPerWindowText)
	{
		FString SamplesString = TEXT("Samples per window: ");
		SamplesString.AppendInt(SamplesPerWindow);
		ConsoleStrings.Emplace(SamplesString);
	}
	else
	{
		ConsoleStrings.Emplace(TEXT(" "));
	}

	ConsoleStrings.Emplace(MessageString);

	Super::RefreshConsole();
}

void UDepthSensorProgram::IncreaseSamplesPerWindow()
{
	if (SamplesPerWindow < MaxSamplesPerWindow)
	{
		SamplesPerWindow++;
		ClearMessageString();
		HideSamplesPerWindowText = false;
		RefreshConsole();
	}
}

void UDepthSensorProgram::DecreaseSamplesPerWindow()
{
	if (SamplesPerWindow > MinSamplesPerWindow)
	{
		SamplesPerWindow--;
		ClearMessageString();
		HideSamplesPerWindowText = false;
		RefreshConsole();
	}
}

void UDepthSensorProgram::LeftKeyPressed()
{
	DecreaseSamplesPerWindow();
}

void UDepthSensorProgram::RightKeyPressed()
{
	IncreaseSamplesPerWindow();
}

void UDepthSensorProgram::CalculateResults()
{
	TArray<int> WindowedData = GetWindowedData(ScanData, SamplesPerWindow);
	EventCount = GetDepthIncreaseEventCount(WindowedData);
}

void UDepthSensorProgram::OperateKeyPressed()
{
	HideSamplesPerWindowText = false;
	if (!LoadDataFromFile(TEXT("DepthScannerData.txt")))
	{
		MessageString = TEXT("Error: Scan failed!<br>Couldn't load scanner data");
		RefreshConsole();
	}
	else
	{
		CalculateResults();
		UE_LOG(LogTemp, Warning, TEXT("EventCount : %i"), EventCount);
		
		MessageString = TEXT("Scanned ");
		MessageString.AppendInt(ScanData.Num());
		MessageString.Append(TEXT(" samples.<br>"));
		MessageString.Append(TEXT("Depth increase event<br>count:"));
		MessageString.AppendInt(EventCount);
		RefreshConsole();
	}
}

void UDepthSensorProgram::TestKeyPressed()
{
	if (!LoadDataFromFile(TEXT("DepthScannerTestData.txt")))
	{
		MessageString = TEXT("Error: Test failed!<br>Couldn't load test data.");
		RefreshConsole();
	}
	else
	{
		int OriginalSamplesPerWindow = SamplesPerWindow;

		SamplesPerWindow = 1;
		CalculateResults();
		int Window1ExpectedEventCount = 7;
		bool Window1Passed = EventCount == Window1ExpectedEventCount;
		
		SamplesPerWindow = 3;
		CalculateResults();
		int Window3ExpectedEventCount = 5;
		bool Window3Passed = EventCount == Window3ExpectedEventCount;
		
		MessageString = TEXT("Tested ");
		MessageString.AppendInt(ScanData.Num());
		MessageString.Append(TEXT(" samples.<br>"));
		
		SamplesPerWindow = OriginalSamplesPerWindow;

		if( Window1Passed )
		{ 
			MessageString.Append(TEXT("Window 1 test passed.<br>"));
			UE_LOG(LogTemp, Warning, TEXT("Window 1 test passed."));
		}
		else
		{
			MessageString.Append(TEXT("Window 1 test failed!<br>"));
			UE_LOG(LogTemp, Warning, TEXT("Window 1 test failed!."));
		}

		if (Window3Passed)
		{
			MessageString.Append(TEXT("Window 3 test passed.<br>"));
			UE_LOG(LogTemp, Warning, TEXT("Window 3 test passed."));
		}
		else
		{
			MessageString.Append(TEXT("Window 3 test failed!<br>"));
			UE_LOG(LogTemp, Warning, TEXT("Window 3 test failed!."));
		}

		if (Window1Passed && Window3Passed)
		{
			MessageString.Append(TEXT("TEST PASSED"));
			UE_LOG(LogTemp, Warning, TEXT("TEST PASSED"));
		}
		else
		{
			MessageString.Append(TEXT("!!! TEST FAILED !!!"));
			UE_LOG(LogTemp, Warning, TEXT("TEST FAILED!"));
		}
		HideSamplesPerWindowText = true;
		RefreshConsole();
	}
}

TArray<int> UDepthSensorProgram::GetWindowedData(const TArray<int>& InData, int InSamplesPerWindow)
{
	TArray<int> WindowedData;
	if (InSamplesPerWindow < 1 || InSamplesPerWindow > InData.Num())
	{
		MessageString = TEXT("Error: Samples per window<BR>too large!<br>Scan Failed.");
		return WindowedData;
	}
	if (InSamplesPerWindow == 1)
	{
		return InData;
	}

	for (int i = 0; i <= InData.Num() - InSamplesPerWindow; i++)
	{
		int TotalDepth = 0;
		for (int j = 0; j < InSamplesPerWindow; j++)
		{
			TotalDepth += InData[i + j];
		}
		WindowedData.Add(TotalDepth);
	}
	return WindowedData;
}

int UDepthSensorProgram::GetDepthIncreaseEventCount(const TArray<int>& EventData)
{
	int count = 0;
	for (int i = 1; i < EventData.Num(); i++)
	{
		if (EventData[i] > EventData[i - 1])
		{
			count++;
		}
	}
	return count;
}

bool UDepthSensorProgram::LoadDataFromFile(const FString& FileName)
{
	TArray<FString> ScanDataStrings;
	if (!LoadStringsFromFile(ScanDataStrings, FileName))
	{
		MessageString = TEXT("Error!<br>Couldn't load strings from file.<br>Analysis failed.");
		RefreshConsole();
		return false;
	}
	
	ScanData.Reset();
	for (FString FoundString : ScanDataStrings)
	{
		ScanData.Emplace(FCString::Atoi(*FoundString));
	}
	return true;
}