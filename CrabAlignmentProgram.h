// Capulet2kx : Advent of Code 2021

#pragma once

#include "CoreMinimal.h"
#include "ComputerProgram.h"
#include "CrabAlignmentProgram.generated.h"

/**
 * Calculate moste fuel efficient crab alignment
 * Day 7 - Advent of Code 2021, Parts 1 & 2.
 */
UCLASS(Blueprintable)
class AOC2021_API UCrabAlignmentProgram : public UComputerProgram
{
	GENERATED_BODY()

public:
	virtual void ProgramSetup() override;
	
	virtual void OperateKeyPressed() override;
	virtual void TestKeyPressed() override;
	virtual void LeftKeyPressed() override;
	virtual void RightKeyPressed() override;

private: 
	bool LoadDataFromFile(const FString& FileName);
	virtual void RefreshConsole() override;
	void CalculateResult();

	TMap<int, int> CrabPositions;

	int LeastFuelPosition = -1;
	int LeastFuelAmount = -1;

	int SoftwareVersion = 2;
};
