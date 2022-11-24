// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnVehicle.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WheeledVehicleMovementcomponent4w.h"

APawnVehicle::APawnVehicle()
{
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	// Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = MinTireLoad;
	Vehicle4W->MinNormalizedTireLoadFiltered = MinTireLoadFiltered;
	Vehicle4W->MaxNormalizedTireLoad = MaxTireLoad;
	Vehicle4W->MaxNormalizedTireLoadFiltered = MaxTireLoadFiltered;

	// Torque setup
	Vehicle4W->MaxEngineRPM = MaxEngineRPM;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(FirstTorqueMin, FirstTorqueMax);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(SecondTorqueMin, SecondTorqueMax);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(ThirdTorqueMin, ThirdTorqueMax);

	// Adjust the steering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(FirstSteeringMin, FirstSteeringMax);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(SecondSteeringMin, SecondSteeringMax);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(ThirdSteeringMin, ThirdSteeringMax);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = FrontRearSplit;

	// Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = GearAutoBox;
	Vehicle4W->TransmissionSetup.GearSwitchTime = GearSwitchTime;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = GearAutoBoxLatency;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = TargetArmLength;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.0f;
}

void APawnVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APawnVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Throttle", this, &APawnVehicle::ApplyThrottle);
	PlayerInputComponent->BindAxis("Steer", this, &APawnVehicle::ApplySteering);
	PlayerInputComponent->BindAxis("LookUp", this, &APawnVehicle::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APawnVehicle::Turn);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &APawnVehicle::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &APawnVehicle::OnHandbrakeReleased);
}

void APawnVehicle::ApplyThrottle(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void APawnVehicle::ApplySteering(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void APawnVehicle::LookUp(float Val)
{
	if (Val != 0.f)
	{
		AddControllerPitchInput(Val);
	}
}

void APawnVehicle::Turn(float Val)
{
	if (Val != 0.f)
	{
		AddControllerYawInput(Val);
	}
}

void APawnVehicle::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void APawnVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void APawnVehicle::setRespawnLocation(FVector LocToSave)
{

}

FVector APawnVehicle::getRespawnLocation()
{
	return FVector();
}
