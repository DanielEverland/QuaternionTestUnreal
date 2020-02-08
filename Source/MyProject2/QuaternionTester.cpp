// Fill out your copyright notice in the Description page of Project Settings.

#include "QuaternionTester.h"

// Sets default values for this component's properties
UQuaternionTester::UQuaternionTester()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	
}


// Called when the game starts
void UQuaternionTester::BeginPlay()
{
	Super::BeginPlay();

	if (qType.GetValue() == QuatType::Artemis)
	{
		quat = std::unique_ptr<ArtemisQuaternion>(new ArtemisQuaternion());
	}
	else if (qType.GetValue() == QuatType::Unreal)
	{
		quat = std::unique_ptr<UnrealQuaternion>(new UnrealQuaternion());		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Shit fam"));
	}

	if (!quat)
	{
		UE_LOG(LogTemp, Error, TEXT("Shit fam"));
		return;
	}
	
	point = FVector(1, 1, 1);
	quat->FromEuler(FVector(5, 5, 5));
	UE_LOG(LogTemp, Warning, TEXT("Quat: %s"), *(quat->ToString()));

	offset = point;
}


// Called every frame
void UQuaternionTester::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	offset = quat->Rotate(offset);

	GetOwner()->SetActorLocation(point + offset * 100);
}

