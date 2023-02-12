// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnVehicle.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WheeledVehicleMovementcomponent4w.h"
#include "Kismet/GameplayStatics.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printf(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))


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
	

	GravitySystemComponent = CreateDefaultSubobject<UGravitySystemComponent>(TEXT("GravitySystemComponent"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.0f;
}

void APawnVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyAirControl(DeltaTime);
	ConsumeMovementInputVector();
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APawnVehicle::OnJumpPressed);
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HANDBRAKE"));
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void APawnVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void APawnVehicle::OnJumpPressed()
{
	const FVector JumpVector = FVector(1000, 1000, 1000);
	UWheeledVehicleMovementComponent* MovmentComponent = GetVehicleMovementComponent();
	UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent());

	//MovmentComponent->SetJumpAllowed(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("JUMP"));
	//MovmentComponent->UpdatedPrimitive->ComponentVelocity.Z = 1000;
	//MovmentComponent->AddInputVector(JumpVector, 1);
	//GetVehicleMovementComponent()->Velocity = JumpVector;
	//AddMovementInput(JumpVector, 1000, true);
	//ConsumeMovementInputVector();
	//RootPrimitive->AddImpulse(JumpVector);
	RootPrimitive->AddRadialImpulse(GetActorLocation(), 1000, 1000, ERadialImpulseFalloff::RIF_Constant, true);
}

void APawnVehicle::ApplyAirControl(float DeltaTime)
{
	if (UWheeledVehicleMovementComponent4W* MovmentComponent = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovementComponent()))
	{
		FCollisionQueryParams CoQuery;
		CoQuery.AddIgnoredActor(this);

		const FVector StartTrace = GetActorLocation() + FVector(0.f, 0.f, 50.f);
		const FVector EndTrace = GetActorLocation() - FVector(0.f, 0.f, 200.f);

		FHitResult Hit;

		const bool isInAir = !GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, CoQuery);
		const bool isNotGrounded = FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0.1f;

		if (isInAir || isNotGrounded)
		{
			const float ForwardInput = InputComponent->GetAxisValue("Throttle");
			const float RightInput = InputComponent->GetAxisValue("Steer");

			const float AirMovmentForcePitch = 3.f;
			const float AirMovmentForceRoll = !isInAir && isNotGrounded ? 20.f : 3.f;

			if (UPrimitiveComponent* VehicleMesh = MovmentComponent->UpdatedPrimitive)
			{
				const FVector MovmentVector = FVector(RightInput * -AirMovmentForceRoll, ForwardInput * AirMovmentForcePitch, 0.f) * DeltaTime * 200.f;
				const FVector AngularMovment = GetActorRotation().RotateVector(MovmentVector);

				VehicleMesh->SetPhysicsAngularVelocity(AngularMovment, true);
			}
		}
	}

}

void APawnVehicle::setRespawnLocation(FVector LocToSave)
{
	RespawnLocation = LocToSave;
}

FVector APawnVehicle::getRespawnLocation()
{
	return FVector(RespawnLocation);
}

void APawnVehicle::increaseLap()
{
	currentLap++;
	if (currentLap == 1)
	{
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APawnVehicle::startTimer, 1, true);
	}
	if (currentLap == maxCurrentLap)
	{
		endGame();
	}
}

int APawnVehicle::getCurrentLap()
{
	return currentLap;
}

void APawnVehicle::endGame()
{
	print("ENDGAME");
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"), true);
}

void APawnVehicle::startTimer()
{
	seconds++;
    // printf("seconds : %d", seconds);
}
