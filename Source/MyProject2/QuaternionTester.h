// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <sstream>
#include <string>
#include <memory>

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Quat.h"
#include "QuaternionTester.generated.h"

using std::stringstream;

UENUM()
enum QuatType
{
	None		UMETA(DisplayName = "None"),
	Unreal		UMETA(DisplayName = "Unreal"),
	Artemis		UMETA(DisplayName = "Artemis"),
};

class QuaternionBase
{
public:
	void virtual FromEuler(FVector vector) = 0;
	FVector virtual Rotate(FVector vector) const = 0;
	FString virtual ToString() const = 0;
};

class UnrealQuaternion : public QuaternionBase
{
public:
	FQuat quat;

	void FromEuler(FVector vector) override
	{
		quat = quat.MakeFromEuler(vector);
	}
	FVector Rotate(FVector vector) const override
	{
		return quat * vector;
	}
	FString ToString() const override
	{
		return "Unreal: " + quat.ToString();
	}
};

class ArtemisQuaternion : public QuaternionBase
{
public:
	double X;
	double Y;
	double Z;
	double W;

	void virtual FromEuler(FVector vector) override
	{
		vector.X = FMath::DegreesToRadians(vector.X);
		vector.Y = FMath::DegreesToRadians(vector.Y);
		vector.Z = FMath::DegreesToRadians(vector.Z);

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
	FVector Rotate(FVector point) const override
	{
		double x = X * 2;
		double y = Y * 2;
		double z = Z * 2;
		double xx = X * x;
		double yy = Y * y;
		double zz = Z * z;
		double xy = X * y;
		double xz = X * z;
		double yz = Y * z;
		double wx = W * x;
		double wy = W * y;
		double wz = W * z;

		FVector res;
		res.X = (1.0 - (yy + zz)) * point.X + (xy - wz) * point.Y + (xz + wy) * point.Z;
		res.Y = (xy + wz) * point.X + (1.0 - (xx + zz)) * point.Y + (yz - wx) * point.Z;
		res.Z = (xz - wy) * point.X + (yz + wx) * point.Y + (1.0 - (xx + yy)) * point.Z;
		return res;
	}
	FString virtual ToString() const override
	{
		stringstream s{};

		s << "Artemis: ";

		s << "(";
		s << X << ", ";
		s << Y << ", ";
		s << Z << ", ";
		s << W << ", ";
		s << ")";

		return s.str().c_str();
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

	std::unique_ptr<QuaternionBase> quat;
	FVector point;
	FVector offset;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
