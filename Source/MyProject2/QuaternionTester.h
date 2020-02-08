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
	FVector virtual ToEuler() const = 0;
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
		FVector euler = quat.Euler();

		UE_LOG(LogTemp, Warning, TEXT("%f"), euler.X);
		UE_LOG(LogTemp, Warning, TEXT("%f"), euler.Y);
		UE_LOG(LogTemp, Warning, TEXT("%f"), euler.Z);

		return "Unreal: " + quat.ToString() + "\n" + quat.Euler().ToString();
	}
	FVector ToEuler() const override
	{
		return quat.Euler();
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

		X =  xCos * ySin * zSin - xSin * yCos * zCos;
		Y = -xCos * ySin * zCos - xSin * yCos * zSin;
		Z =  xCos * yCos * zSin - xSin * ySin * zCos;
		W =  xCos * yCos * zCos + xSin * ySin * zSin;
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
	FVector ToEuler() const override
	{
		const float SingularityTest = Z * X - W * Y;
		const float YawY = 2.f * (W * Z + X * Y);
		const float YawX = (1.f - 2.f * (FMath::Square(Y) + FMath::Square(Z)));

		const float SINGULARITY_THRESHOLD = 0.4999995f;
		const float RAD_TO_DEG = (180.f) / PI;
		FVector euler;

		if (SingularityTest < -SINGULARITY_THRESHOLD)
		{
			euler.Y = -90.f;
			euler.Z = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
			euler.X = FRotator::NormalizeAxis(-euler.Z - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
		}
		else if (SingularityTest > SINGULARITY_THRESHOLD)
		{
			euler.Y = 90.f;
			euler.Z = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
			euler.X = FRotator::NormalizeAxis(euler.Z - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
		}
		else
		{
			euler.Y = FMath::FastAsin(2.f * (SingularityTest)) * RAD_TO_DEG;
			euler.Z = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
			euler.X = FMath::Atan2(-2.f * (W * X + Y * Z), (1.f - 2.f * (FMath::Square(X) + FMath::Square(Y)))) * RAD_TO_DEG;
		}

		return euler;
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

		s << "\n";

		FVector euler = ToEuler();

		s << "(";
		s << euler.X << ", ";
		s << euler.Y << ", ";
		s << euler.Z << ", ";
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
