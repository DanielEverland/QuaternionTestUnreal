// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Quat.h"
#include "QuaternionTester.generated.h"

UENUM()
enum QuatType
{
	None		UMETA(DisplayName = "None"),
	Unreal		UMETA(DisplayName = "Unreal"),
	Artemis		UMETA(DisplayName = "Artemis"),
};

class Test
{
public:
	void virtual FromEuler(FVector vector) = 0;
};

class UnrealQuaternion : public Test
{
public:
	FQuat quat;

	void virtual FromEuler(FVector vector)
	{
		quat.MakeFromEuler(vector);
	}
};

class ArtemisQuaternion : public Test
{
public:
	double X;
	double Y;
	double Z;
	double W;

	void virtual FromEuler(FVector vector)
	{
		vector.X = FMath::RadiansToDegrees(vector.X);
		vector.Y = FMath::RadiansToDegrees(vector.Y);
		vector.Z = FMath::RadiansToDegrees(vector.Z);

		// Abbreviations for the various angular functions
		double xCos = FMath::Cos(vector.X / 2);
		double xSin = FMath::Sin(vector.X / 2);
		double yCos = FMath::Cos(vector.Y / 2);
		double ySin = FMath::Sin(vector.Y / 2);
		double zCos = FMath::Cos(vector.Z / 2);
		double zSin = FMath::Sin(vector.Z / 2);

		X = ySin * zSin * xCos + yCos * zCos * xSin;
		Y = ySin * zCos * xCos + yCos * zSin * xSin;
		Z = yCos * zSin * xCos - ySin * zCos * xSin;
		W = yCos * zCos * xCos - ySin * zSin * xSin;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT2_API UQuaternionTester : public UActorComponent
{
	

	GENERATED_BODY()

public:	

	

	// Sets default values for this component's properties
	UQuaternionTester();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	TEnumAsByte<QuatType> qType;

	//Test* quat;
	FVector point;
	FVector offset;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
