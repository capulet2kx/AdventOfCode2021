// Capulet2kx : Advent of Code 2021

#pragma once

#include "CoreMinimal.h"
#include "ArrayStructs.generated.h"

// 2d arrays aren't blueprint friendly,
// but an array of a struct that contains an array is okay.
// Here's a bunch of such structs

USTRUCT()
struct FVector2DArray
{
	GENERATED_BODY()
public:
	TArray<FVector2D> Coordinates;
};

USTRUCT()
struct FIntArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TArray<int> IntArray;
};

USTRUCT()
struct FBoolArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TArray<bool> BoolArray;
};

USTRUCT()
struct FStringArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TArray<FString> StringArray;
};
