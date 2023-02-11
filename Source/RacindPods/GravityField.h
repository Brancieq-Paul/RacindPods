// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attractor.h"
#include "Components/ArrowComponent.h"
#include "GravityField.generated.h"

UCLASS(Abstract, ClassGroup = (GravitySystem), Blueprintable)
class AGravityField : public AVolume
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "GravitySystem", EditDefaultsOnly, BlueprintReadOnly)
		UArrowComponent* ArrowComponent;

	UPROPERTY(Category = "GravitySystem", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAttractor* attractor;
	
	UPROPERTY(Category = "GravitySystem", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float GravityStrength;

	UPROPERTY(Category = "GravitySystem", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Priority;

	UFUNCTION(BlueprintCallable, Category = "GravitySystem")
		virtual FVector GetForce(FVector position);
	
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

public:	
	// Sets default values for this actor's properties
	AGravityField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void UpdateDebugArrowDirection();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
