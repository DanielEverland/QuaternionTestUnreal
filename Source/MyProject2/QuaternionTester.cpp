// Fill out your copyright notice in the Description page of Project Settings.

#include "QuaternionTester.h"

// Sets default values for this component's properties
UQuaternionTester::UQuaternionTester()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	//switch (qType.GetValue())
	//{
	//case Unreal:
	//	quat = new UnrealQuaternion();
	//case Artemis:
	//	quat = new ArtemisQuaternion();
	////default:
	//	//UE_LOG(LogTemp, Error, TEXT("Shit fam"));
	//}

	//point = FVector(1, 1, 1);
	//quat = quat.MakeFromEuler(FVector(0, 0, 5));
	//offset = point;
}


// Called when the game starts
void UQuaternionTester::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UQuaternionTester::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//offset = quat * offset;

	//GetOwner()->SetActorLocation(point + offset * 100);
	
	//UE_LOG(LogTemp, Display, TEXT("Quat: %s"), *offset.ToString());
}

