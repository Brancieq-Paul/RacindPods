// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityField.h"
#include "GravitySystemComponent.h"

// Sets default values
AGravityField::AGravityField() : attractor(0), GravityStrength(980)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bGenerateOverlapEventsDuringLevelStreaming = true;
	
	OnActorBeginOverlap.AddDynamic(this, &AGravityField::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AGravityField::OnOverlapEnd);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	UpdateDebugArrowDirection();
}

// Called when the game starts or when spawned
void AGravityField::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGravityField::UpdateDebugArrowDirection() {
	FVector Force = GetForce(FVector(ArrowComponent->GetComponentLocation()));
	FRotator ForceRotation = Force.Rotation();
	ArrowComponent->SetRelativeRotation(ForceRotation);
}

FVector AGravityField::GetForce(FVector position)
{
	if (!attractor) return FVector::ZeroVector;
	FVector direction = attractor->GetAttractionPoint(position) - position;
	direction.Normalize();
	return direction * GravityStrength;
}

void AGravityField::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, TEXT("BeginOverlap"));
	UGravitySystemComponent *GravitySystem = OtherActor->FindComponentByClass<UGravitySystemComponent>();
	if (GravitySystem)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, TEXT("GravityField entered"));
		GravitySystem->OverlappingGravityFields.Add(this);
	}
}

void AGravityField::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, TEXT("EndOverlap"));
	UGravitySystemComponent* GravitySystem = OtherActor->FindComponentByClass<UGravitySystemComponent>();
	if (GravitySystem)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, TEXT("GravityField exited"));
		GravitySystem->OverlappingGravityFields.Remove(this);
	}
}
