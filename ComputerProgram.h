// Capulet2kx : Advent of Code 2021

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../ArrayStructs.h"
#include "ComputerProgram.generated.h"

// The computer program will broadcast when it's output console strings have changed.
DECLARE_DELEGATE(FConsoleStringsChanged);

/** 
 * Parent class for ComputerPrograms.
 * Reusable functionality, and providing input functions to override.
 * Console strings for drawing to the computer display, and an event 
 * to let the computer know when to redraw the display.
 */
UCLASS()
class AOC2021_API UComputerProgram : public UObject
{
	GENERATED_BODY()
	
public:
	
	FText ProgramName;

	// The computer listens for this to know it needs to update the display.
	FConsoleStringsChanged OnConsoleStringsChanged;

	UFUNCTION(BlueprintCallable)
		const TArray<FString>& GetConsoleStrings();

	// The computer will call this after instantiating the program
	virtual void ProgramSetup();

	// The computer will call these when the buttons are pressed
	virtual void LeftKeyPressed();
	virtual void RightKeyPressed();
	virtual void OperateKeyPressed();
	virtual void TestKeyPressed();

	// Call this to refresh the console strings (e.g. with a changed message string)
	// and broadcast that the console strings have changed.
	virtual void RefreshConsole();
			
protected:
	UPROPERTY()
		TArray<FString> ConsoleStrings;

	// The message string is used to populate the console strings with results.
	// (while e.g. the displayed program name remains the same)
	UPROPERTY()
		FString MessageString = TEXT("");

	// Clearing the message string.
	void ClearMessageString();

	// Loading an input text file per line to a list of strings
	const bool LoadStringsFromFile(TArray<FString>& ResultStrings, const FString& FileName);

	// Convert a TCHAR to a string
	const FString CharToString(const TCHAR& InChar);

	// Count the unique characters in a string.
	const int GetUniqueCharCount(const FString& InString);
};
