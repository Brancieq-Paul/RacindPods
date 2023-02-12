// Fill out your copyright notice in the Description page of Project Settings.


#include "AttractionPoint.h"

// Sets default values for this component's properties
UAttractionPoint::UAttractionPoint()
{
	// ...
}

FVector UAttractionPoint::GetAttractionPoint(FVector position)
{
	return GetComponentLocation();
}