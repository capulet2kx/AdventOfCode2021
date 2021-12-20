// Capulet2kx : Advent of Code 2021

#pragma once

#include "CoreMinimal.h"
#include "ComputerProgram.h"
#include "DepthSensorProgram.generated.h"

/**
 * Analyse the readings from the depth sensor, with variable samples per window.
 * Day 1 - Advent of Code 2021, Parts 1 & 2.
 */

UCLASS(Blueprintable)
class AOC2021_API UDepthSensorProgram : public UComputerProgram
{
	GENERATED_BODY()
	
public:

		virtual void ProgramSetup() override;

		virtual void RefreshConsole() override;

		virtual void LeftKeyPressed() override;
		virtual void RightKeyPressed() override;
		virtual void OperateKeyPressed() override;
		virtual void TestKeyPressed() override;

protected:
	bool LoadDataFromFile(const FString& FileName);
	void CalculateResults();
	
	const int MinSamplesPerWindow = 1;
	const int MaxSamplesPerWindow = 20;
	
	int SamplesPerWindow = 1;
	int EventCount = 0;

	void IncreaseSamplesPerWindow();
	void DecreaseSamplesPerWindow();

	UPROPERTY()
		TArray<int> ScanData;
		
	TArray<int> GetWindowedData(const TArray<int>& InData, int InSamplesPerWindow);
	int GetDepthIncreaseEventCount(const TArray<int>& EventData);
	bool HideSamplesPerWindowText = false;
};
