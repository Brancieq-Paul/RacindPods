// Fill out your copyright notice in the Description page of Project Settings.


#include "LinearGravityField.h"
#include "AttractionPoint.h"

ALinearGravityField::ALinearGravityField() : AGravityField()
{
	UpdateDebugArrowDirection();
}

FVector ALinearGravityField::GetForce(FVector position)
{
	return GetActorUpVector() * -1 * GravityStrength;
}