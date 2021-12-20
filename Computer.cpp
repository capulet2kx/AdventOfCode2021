// Fill out your copyright notice in the Description page of Project Settings.


#include "Computer.h"
#include "ComputerPrograms/ComputerProgram.h"

// Sets default values
AComputer::AComputer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AComputer::BeginPlay()
{
	Super::BeginPlay();
	if (ComputerProgramClass != nullptr)
	{
		LoadComputerProgramByClass(ComputerProgramClass);
	}
}

// Called every frame
void AComputer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AComputer::LoadComputerProgramByClass(TSubclassOf<UComputerProgram> ComputerProgamClass)
{
	ComputerProgram = NewObject<UComputerProgram>(this, ComputerProgamClass);
	if (ComputerProgram)
	{
		ComputerProgram->ProgramSetup();
		ComputerProgram->OnConsoleStringsChanged.BindUObject(this, &AComputer::OnTextOutputChanged);
		OnTextOutputChanged();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AComputer::BeginPlay() : ComputerProgram == nullptr! "));
	}
}



void AComputer::LeftButtonPressed()
{
	if (ComputerProgram)
	{
		ComputerProgram->LeftKeyPressed();
	}
}


void AComputer::RightButtonPressed()
{
	if (ComputerProgram)
	{
		ComputerProgram->RightKeyPressed();
	}
}


void AComputer::OperateButtonPressed()
{
	if (ComputerProgram)
	{
		ComputerProgram->OperateKeyPressed();
	}
}


void AComputer::TestButtonPressed()
{
	if (ComputerProgram)
	{
		ComputerProgram->TestKeyPressed();
	}
}