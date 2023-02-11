// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Attractor.h"
#include "AttractionPoint.generated.h"


UCLASS( ClassGroup=(GravitySystem), meta=(BlueprintSpawnableComponent) )
class UAttractionPoint : public UAttractor
{
	GENERATED_BODY()
		
public:
	virtual FVector GetAttractionPoint(FVector position) override;

public:
	UAttractionPoint();
		
};
