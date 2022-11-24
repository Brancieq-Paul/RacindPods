// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointBox.h"
#include "PawnVehicle.h"
#include "DrawDebugHelpers.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printf(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

ACheckpointBox::ACheckpointBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ACheckpointBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACheckpointBox::OnOverlapEnd);
}

void ACheckpointBox::BeginPlay()
{
	Super::BeginPlay();
}

void ACheckpointBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (APawnVehicle* Vehicle = Cast<APawnVehicle>(OtherActor))
	{
		printf("Overlapped Actor = %s", *OtherActor->GetName());
		Vehicle->setRespawnLocation(this->GetActorLocation());
	}
}

void ACheckpointBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{

}
