// Capulet2kx : Advent of Code 2021

#pragma once

#include "CoreMinimal.h"
#include "ComputerProgram.h"
#include "HydrothermalAvoidanceProgram.generated.h"


// The X axis coordinate and the value at the coordinate
// Each used Y axis gets one of these maps, making a 2D map
USTRUCT()
struct FCoordinateMap
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TMap<int,int> XAxis;
};

// Counts the number of hits on a coordiante.
USTRUCT()
struct FCoordinateCounter
{
	GENERATED_BODY()
public:
	// Each Y axis value maps to a CoordinateMap, wherein the X axis value maps to the data.
	UPROPERTY()
		TMap<int,FCoordinateMap> YAxis;

	void ResetCoordinateCount();

	// Draw a line, increasing the count in each coord hit by 1
	void DrawLine(const FVector2D& Start, const FVector2D& End, int SoftwareVersion);

	// Increase the count in a particular coord by 1
	void AddToCoordinateCount(FVector2D InCoordinate);

	// Get a list of coordinate between certain values
	void FilterCoordinatesByCount(TArray<FVector2D>& Coordinates, int MinCountInclusive, int MaxCountInclusive);
};



UCLASS(Blueprintable)
class AOC2021_API UHydrothermalAvoidanceProgram : public UComputerProgram
{
	GENERATED_BODY()
	
public:
	virtual void ProgramSetup() override;
	
	virtual void OperateKeyPressed() override;
	virtual void TestKeyPressed() override;
	virtual void LeftKeyPressed() override;
	virtual void RightKeyPressed() override;

private:
	virtual void RefreshConsole() override;
	bool LoadDataFromFile(const FString& FileName);

	void CalculateResult();

	int SoftwareVersion = 2;
	int OverlapPoints = 0;

	UPROPERTY()
	FCoordinateCounter CoordinateCounter;

	UPROPERTY()
		TArray<FVector2DArray> StartEndCoordinateData;
};
