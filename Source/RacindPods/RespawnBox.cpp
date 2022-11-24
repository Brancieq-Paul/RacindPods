// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnBox.h"
#include "PawnVehicle.h"
#include "DrawDebugHelpers.h"

ARespawnBox::ARespawnBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ARespawnBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ARespawnBox::OnOverlapEnd);
}

void ARespawnBox::BeginPlay()
{
	Super::BeginPlay();
}

void ARespawnBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{

}

void ARespawnBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{

}