// Capulet2kx : Advent of Code 2021

#include "ComputerProgram.h"

void UComputerProgram::ProgramSetup()
{
	ProgramName = FText::FromString(TEXT("Unknown System"));
}

const TArray<FString>& UComputerProgram::GetConsoleStrings()
{
	return ConsoleStrings;
}

void UComputerProgram::RefreshConsole()
{
	OnConsoleStringsChanged.ExecuteIfBound();
}

void UComputerProgram::LeftKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftKeyPressed() not handled by program!"));
}

void UComputerProgram::RightKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("RightKeyPressed() not handled by program!"));
}

void UComputerProgram::OperateKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("ScanKeyPressed() not handled by program!"));
}

void UComputerProgram::TestKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("TestKeyPressed() not handled by program!"));
}

void UComputerProgram::ClearMessageString()
{
	MessageString = TEXT("");
}


const bool UComputerProgram::LoadStringsFromFile(TArray<FString>& ResultStrings, const FString& FileName)
{
	const FString Path = FPaths::ProjectContentDir() / TEXT("Data") / FileName;
	ResultStrings.Reset();
	return FFileHelper::LoadFileToStringArray(ResultStrings, *Path);
}

const FString UComputerProgram::CharToString(const TCHAR& InChar)
{
	TArray<TCHAR> CharArray;
	CharArray.Add(InChar);
	return FString(CharArray);
}

const int UComputerProgram::GetUniqueCharCount(const FString& InString)
{
	TMap<TCHAR, int> CharCounts;
	TArray<TCHAR> Chars = InString.GetCharArray();
	
	for (TCHAR FoundChar : Chars)
	{
		CharCounts.FindOrAdd(FoundChar);
	}
	// The final character is a terminator, reduce character count by 1
	return CharCounts.Num() - 1;
}