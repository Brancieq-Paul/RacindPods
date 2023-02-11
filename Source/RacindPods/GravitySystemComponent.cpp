// Fill out your copyright notice in the Description page of Project Settings.

#include "GravitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UGravitySystemComponent::UGravitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetAutoActivate(true);
}


// Called when the game starts
void UGravitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the collider and assign OnComponentBeginOverlap and OnComponentEndOverlap functions
	Collider = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	
	if (Collider)
	{
		Collider->SetEnableGravity(false);
	}
}


// Called every frame
void UGravitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the strongest gravity fields
	TArray<AGravityField*> StrongestGravityFields = SelectStrongestGravityFields();
	TArray<FVector> Forces;
	
	for (AGravityField* GravityField : StrongestGravityFields)
	{
		Forces.Add(GravityField->GetForce(GetOwner()->GetActorLocation()));
	}
	FVector AverageForce = UKismetMathLibrary::GetVectorArrayAverage(Forces);
	CurrentGravityDirection = AverageForce.GetSafeNormal();
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.1f, FColor::Yellow, AverageForce.ToString());
	Collider->AddForce(AverageForce, NAME_None, true);
}

TArray<AGravityField *> UGravitySystemComponent::SelectStrongestGravityFields() {
	TArray<AGravityField *> StrongestGravityFields;
	float MaxWeight = 0;
	for (AGravityField* GravityField : OverlappingGravityFields) {
		if (GravityField->Weight > MaxWeight) {
			MaxWeight = GravityField->Weight;
			StrongestGravityFields.Empty();
			StrongestGravityFields.Add(GravityField);
		}
		else if (GravityField->Weight == MaxWeight) {
			StrongestGravityFields.Add(GravityField);
		}
	}
	return StrongestGravityFields;
}

FVector UGravitySystemComponent::GetCurrentGravityDirection() {
	return CurrentGravityDirection;
}