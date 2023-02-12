// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "PawnVehicle.generated.h"

/**
 * 
 */
UCLASS()
class RACINDPODS_API APawnVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
	
public:
	APawnVehicle();

	virtual void Tick(float DeltaTime) override; // called every frame 

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Moving forward / right
	void ApplyThrottle(float Val); // Val = how much force we want to apply
	void ApplySteering(float Val);

	void LookUp(float Val);
	void Turn(float Val);

	void OnHandbrakePressed();
	void OnHandbrakeReleased();

	void OnJumpPressed();

	void ApplyAirControl(float DeltaTime);

	void setRespawnLocation(FVector LocToSave);
	FVector getRespawnLocation();

	void increaseLap();
	UFUNCTION(Category = Race, BlueprintCallable, BlueprintPure)
		int getCurrentLap();

	void endGame();
	void startTimer();

	// Timer variables
	UPROPERTY(Category = Timer, EditDefaultsOnly, BlueprintReadOnly)
		int seconds;

	// Camera variables
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	// Tire variables
	UPROPERTY(Category = Tire, EditDefaultsOnly, BlueprintReadOnly)
		float MinTireLoad;
	UPROPERTY(Category = Tire, EditDefaultsOnly, BlueprintReadOnly)
		float MinTireLoadFiltered = 0.2f;
	UPROPERTY(Category = Tire, EditDefaultsOnly, BlueprintReadOnly)
		float MaxTireLoad = 2.0f;
	UPROPERTY(Category = Tire, EditDefaultsOnly, BlueprintReadOnly)
		float MaxTireLoadFiltered = 2.0f;

	// Torque variables
	UPROPERTY(Category = Torque, EditDefaultsOnly, BlueprintReadOnly)
		float MaxEngineRPM = 5700.0f;
	UPROPERTY(Category = Torque, EditDefaultsOnly, BlueprintReadOnly)
		float FirstTorqueMin = 0.0f;
	UPROPERTY(Category = Torque, EditDefaultsOnly, BlueprintReadOnly)
		float FirstTorqueMax = 400.0f;
	UPROPERTY(Category = Torque, EditDefaultsOnly, BlueprintReadOnly)
		float SecondTorqueMin = 1890.0f;
	UPROPERTY(Category = Torque, EditDefaultsOnly, BlueprintReadOnly)
		float SecondTorqueMax = 500.0f;
	UPROPERTY(Category = Torque, EditDefaultsOnly, BlueprintReadOnly)
		float ThirdTorqueMin = 5730.0f;
	UPROPERTY(Category = Torque, EditDefaultsOnly, BlueprintReadOnly)
		float ThirdTorqueMax = 400.0f;

	// Steering variables
	UPROPERTY(Category = Steering, EditDefaultsOnly, BlueprintReadOnly)
		float FirstSteeringMin = 0.0f;
	UPROPERTY(Category = Steering, EditDefaultsOnly, BlueprintReadOnly)
		float FirstSteeringMax = 1.0f;
	UPROPERTY(Category = Steering, EditDefaultsOnly, BlueprintReadOnly)
		float SecondSteeringMin = 40.0f;
	UPROPERTY(Category = Steering, EditDefaultsOnly, BlueprintReadOnly)
		float SecondSteeringMax = 0.7f;
	UPROPERTY(Category = Steering, EditDefaultsOnly, BlueprintReadOnly)
		float ThirdSteeringMin = 120.0f;
	UPROPERTY(Category = Steering, EditDefaultsOnly, BlueprintReadOnly)
		float ThirdSteeringMax = 0.6f;
	UPROPERTY(Category = Steering, EditDefaultsOnly, BlueprintReadOnly)
		float FrontRearSplit = 0.65f;

	// Gearbox variables
	UPROPERTY(Category = Gearbox, EditDefaultsOnly, BlueprintReadOnly)
		bool GearAutoBox = true;
	UPROPERTY(Category = Gearbox, EditDefaultsOnly, BlueprintReadOnly)
		float GearSwitchTime = 0.15f;
	UPROPERTY(Category = Gearbox, EditDefaultsOnly, BlueprintReadOnly)
		float GearAutoBoxLatency = 1.0f;

	// Race variables
	UPROPERTY(Category = Race, EditAnywhere, BlueprintReadOnly)
		int maxCurrentLap = 3;
		
private:
	FVector RespawnLocation;
	int currentLap = 0;
};
