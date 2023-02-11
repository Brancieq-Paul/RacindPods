// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityField.h"
#include "LinearGravityField.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (GravitySystem), Blueprintable)
class ALinearGravityField : public AGravityField
{
	GENERATED_BODY()

public:
	FVector GetForce(FVector position) override;
		
public:
	ALinearGravityField();
};
