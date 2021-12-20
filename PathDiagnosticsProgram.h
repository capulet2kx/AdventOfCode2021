// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComputerProgram.h"
#include "PathDiagnosticsProgram.generated.h"

/**
 * Analyse the path data to understand how to pilot the submarine.
 * Day 2 - Advent of Code 2021, Parts 1 & 2.
 */
UCLASS(Blueprintable)
class AOC2021_API UPathDiagnosticsProgram : public UComputerProgram
{
	GENERATED_BODY()
	
public:
	virtual void ProgramSetup() override;

	virtual void RefreshConsole() override;

	virtual void LeftKeyPressed() override;
	virtual void RightKeyPressed() override;
	virtual void OperateKeyPressed() override;
	virtual void TestKeyPressed() override;

	bool LoadDataFromFile(const FString& FileName);
	void CalculateResultsV1();
	void CalculateResultsV2();

private:

	TArray<TPair<FString, int>> PathData;
	
	int FinalDepth = 0;
	int FinalHorizontalDistance = 0;
	int MultipliedFinalValues = 0;

	int SoftwareVersion = 2;
};
