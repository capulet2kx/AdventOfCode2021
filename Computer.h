// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"
#include "Computer.generated.h"

class UComputerProgram;

UCLASS()
class AOC2021_API AComputer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComputer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LoadComputerProgramByClass(TSubclassOf<UComputerProgram> ComputerProgamClass);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UComputerProgram> ComputerProgramClass;

	UPROPERTY(BlueprintReadOnly)
	UComputerProgram* ComputerProgram;
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnTextOutputChanged();

	UFUNCTION(BlueprintCallable)
		void LeftButtonPressed();

	UFUNCTION(BlueprintCallable)
		void RightButtonPressed();

	UFUNCTION(BlueprintCallable)
		void OperateButtonPressed();

	UFUNCTION(BlueprintCallable)
		void TestButtonPressed();

};
