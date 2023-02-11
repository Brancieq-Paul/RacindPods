// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Attractor.generated.h"


UCLASS(Abstract)
class UAttractor : public USceneComponent
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
		virtual FVector GetAttractionPoint(FVector position) PURE_VIRTUAL(UAttractor::GetAttractionPoint, return FVector(););

public:	
	// Sets default values for this component's properties
	UAttractor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
