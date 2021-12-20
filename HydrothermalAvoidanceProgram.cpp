// Capulet2kx : Advent of Code 2021


#include "HydrothermalAvoidanceProgram.h"

void FCoordinateCounter::ResetCoordinateCount()
{
	YAxis.Reset();
}

void FCoordinateCounter::AddToCoordinateCount(FVector2D InCoordinate)
{
	if (!YAxis.Contains(InCoordinate.Y))
	{
		YAxis.Emplace(InCoordinate.Y, FCoordinateMap());
	}
	if (!(YAxis[InCoordinate.Y].XAxis.Contains(InCoordinate.X)))
	{
		YAxis[InCoordinate.Y].XAxis.Emplace(InCoordinate.X, 0);
	}
	YAxis[InCoordinate.Y].XAxis[InCoordinate.X]++;
}

void FCoordinateCounter::FilterCoordinatesByCount(TArray<FVector2D>& Coordinates, int MinCountInclusive, int MaxCountInclusive)
{
	if (MinCountInclusive > MaxCountInclusive)
	{
		int Temp = MaxCountInclusive;
		MaxCountInclusive = MinCountInclusive;
		MinCountInclusive = Temp;
	}

	TArray<int> YKeys;
	YAxis.GetKeys(YKeys);

	for (int CoordY : YKeys)
	{
		TArray<int> XKeys;
		YAxis[CoordY].XAxis.GetKeys(XKeys);
		for (int CoordX : XKeys)
		{
			int FoundCount = YAxis[CoordY].XAxis[CoordX];
			if (FoundCount >= MinCountInclusive && FoundCount <= MaxCountInclusive)
			{
				FVector2D NewCoord(CoordX, CoordY);
				Coordinates.Emplace(NewCoord);
			}
		}
	}
}

void FCoordinateCounter::DrawLine(const FVector2D& Start, const FVector2D& End, int SoftwareVersion)
{
	// Only draw vertical or horizontal lines
	if (Start.X == End.X)
	{
		int Min = Start.Y;
		int Max = End.Y;
		if (Min > Max)
		{
			Max = Start.Y;
			Min = End.Y;
		}
		
		FVector2D NewCoord;
		NewCoord.X = Start.X;
		for (int i = Min; i <= Max; i++)
		{
			NewCoord.Y = i;
			AddToCoordinateCount(NewCoord);
		}
	}
	else if (Start.Y == End.Y)
	{
		int Min = Start.X;
		int Max = End.X;
		if (Min > Max)
		{
			Max = Start.X;
			Min = End.X;
		}

		FVector2D NewCoord;
		NewCoord.Y = Start.Y;
		for (int i = Min; i <= Max; i++)
		{
			NewCoord.X = i;
			AddToCoordinateCount(NewCoord);
		}
	}
	// V2 also handles diagonals.  Remaining paths are guaranteed to be diagonal.
	else
	{
		if (SoftwareVersion == 2)
		{
			// Direction to move each axis
			int XDir = 1;
			if( Start.X > End.X )
			{
				XDir = -1;
			}
			int YDir = 1;
			if( Start.Y > End.Y )
			{
				YDir = -1;
			}

			FVector2D NewCoord;
			int x = Start.X;
			int y = Start.Y;
			int Distance = FMath::Abs(Start.X - End.X);
			for (int i = 0; i <= Distance; i++)
			{
				NewCoord.X = x;
				NewCoord.Y = y;
				AddToCoordinateCount(NewCoord);
				x += XDir;
				y += YDir;
			}
		}
	}
}


void UHydrothermalAvoidanceProgram::ProgramSetup()
{
	Super::ProgramSetup();
	ProgramName = FText::FromString(TEXT("Hydrothermals"));
	MessageString = TEXT("System online.");
	RefreshConsole();
}

void UHydrothermalAvoidanceProgram::RefreshConsole()
{
	ConsoleStrings.Reset();
	if (SoftwareVersion == 1)
	{
		ConsoleStrings.Emplace(TEXT("Hydrothermals V1"));
	}
	else if (SoftwareVersion == 2)
	{
		ConsoleStrings.Emplace(TEXT("Hydrothermals V2"));
	}
	else
	{
		ConsoleStrings.Emplace(TEXT("Hydrothermals V?"));
	}
	ConsoleStrings.Emplace(TEXT("============"));
	ConsoleStrings.Emplace(MessageString);
	Super::RefreshConsole();
}

void UHydrothermalAvoidanceProgram::CalculateResult()
{
	CoordinateCounter.ResetCoordinateCount();
	OverlapPoints = 0;
	for (FVector2DArray FoundCoordinateArray : StartEndCoordinateData)
	{
		CoordinateCounter.DrawLine(FoundCoordinateArray.Coordinates[0], FoundCoordinateArray.Coordinates[1], SoftwareVersion);
	}
	TArray<FVector2D> FilteredCoordinates;
	CoordinateCounter.FilterCoordinatesByCount(FilteredCoordinates, 2, 100000);
	OverlapPoints = FilteredCoordinates.Num();
}

void UHydrothermalAvoidanceProgram::TestKeyPressed()
{
	if (!LoadDataFromFile(TEXT("HydrothermalAvoidanceTestData.txt")))
	{
		MessageString = TEXT("Error!<br>Couldn't load data.<br>Test failed.");
		RefreshConsole();
		return;
	}

	MessageString = TEXT("Loaded test data.<br>");

	if (SoftwareVersion == 1)
	{
		CalculateResult();

		if (OverlapPoints == 5)
		{
			UE_LOG(LogTemp, Warning, TEXT("1 test passed. V1 Test Success."));
			MessageString.Append(TEXT("1 test passed.<br>V1 Test Success."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error! Unexpected value. V1 Test failed."));
			MessageString.Append(TEXT("Error!<br>Unexpected value.<br>V1 Test failed."));
		}
	}
	else
	{
		CalculateResult();

		if (OverlapPoints == 12)
		{
			UE_LOG(LogTemp, Warning, TEXT("1 test passed. V2 Test Success."));
			MessageString.Append(TEXT("1 test passed.<br>V2 Test Success."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error! Unexpected value %i. V2 Test failed."), OverlapPoints);
			MessageString.Append(TEXT("Error!<br>Unexpected value.<br>V2 Test failed."));
		}
	}

	
	RefreshConsole();
}

void UHydrothermalAvoidanceProgram::OperateKeyPressed()
{
	if (!LoadDataFromFile(TEXT("HydrothermalAvoidanceData.txt")))
	{
		MessageString = TEXT("Error!<br>Couldn't load data.<br>Test failed.");
		RefreshConsole();
		return;
	}

	MessageString = TEXT("Loaded scan data.<br>");

	CalculateResult();

	MessageString.Append(TEXT("Overlap points:<br>"));
	MessageString.AppendInt(OverlapPoints);
	MessageString.Append(TEXT("<br>Analysis complete."));
	RefreshConsole();
	UE_LOG(LogTemp, Warning, TEXT("Overlap points: %i"), OverlapPoints);
}

bool UHydrothermalAvoidanceProgram::LoadDataFromFile(const FString& FileName)
{
	TArray<FString> DataStrings;
	if (!LoadStringsFromFile(DataStrings, FileName))
	{
		MessageString = TEXT("Error!<br>Couldn't load strings from file.<br>Analysis failed.");
		RefreshConsole();
		return false;
	}

	StartEndCoordinateData.Reset();
	for (FString FoundString : DataStrings)
	{
		FString LeftSide, RightSide;
		FoundString.Split(TEXT(" -> "), &LeftSide, &RightSide);
		
		FString StartX, StartY;
		LeftSide.Split(TEXT(","), &StartX, &StartY);
		FVector2D StartCoord;
		StartCoord.X = FCString::Atoi(*StartX);
		StartCoord.Y = FCString::Atoi(*StartY);

		FString EndX, EndY;
		RightSide.Split(TEXT(","), &EndX, &EndY);
		FVector2D EndCoord;
		EndCoord.X = FCString::Atoi(*EndX);
		EndCoord.Y = FCString::Atoi(*EndY);
				
		FVector2DArray NewCoordinateArray;
		NewCoordinateArray.Coordinates.Emplace(StartCoord);
		NewCoordinateArray.Coordinates.Emplace(EndCoord);
		
		StartEndCoordinateData.Emplace(NewCoordinateArray);
	}
	return true;
}

void UHydrothermalAvoidanceProgram::LeftKeyPressed()
{
	if (SoftwareVersion > 1)
	{
		SoftwareVersion--;
		ClearMessageString();
		RefreshConsole();
	}
}

void UHydrothermalAvoidanceProgram::RightKeyPressed()
{
	if (SoftwareVersion < 2)
	{
		SoftwareVersion++;
		ClearMessageString();
		RefreshConsole();
	}
}