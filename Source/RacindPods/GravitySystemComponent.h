// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityField.h"
#include "GravitySystemComponent.generated.h"


UCLASS( ClassGroup=(GravitySystem), meta=(BlueprintSpawnableComponent) )
class UGravitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GravitySystem")
		TArray<AGravityField *> OverlappingGravityFields;
	
	UFUNCTION(BlueprintCallable, Category = "GravitySystem")
		FVector GetCurrentGravityDirection();
	
private:
	FVector CurrentGravityDirection;
	UPrimitiveComponent* Collider;
	TArray<AGravityField *> SelectStrongestGravityFields();

public:
	// Sets default values for this component's properties
	UGravitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
