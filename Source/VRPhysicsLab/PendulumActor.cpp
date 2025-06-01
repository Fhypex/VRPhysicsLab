#include "PendulumActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/InputComponent.h"

// Constructor
APendulumActor::APendulumActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;


	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetEnableGravity(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComp->SetMobility(EComponentMobility::Movable);
	Tags.Add("Grabbable");
}

void APendulumActor::BeginPlay()
{
	Super::BeginPlay();

	InitialPosition = GetActorLocation();
	PivotLocation = InitialPosition + FVector(0.0f, 0.0f, 60.6f);

	TheoreticalPeriod = 2 * PI * FMath::Sqrt(CableLength / Gravity);
	float DampingCoefficient = 0.2f;
	float Gamma = DampingCoefficient / Mass;
	float DampedPeriod = TheoreticalPeriod * (1 + (Gamma * Gamma) / 16.0f);

	UE_LOG(LogTemp, Log, TEXT("Theoretical Period: %.2f seconds"), TheoreticalPeriod);
	UE_LOG(LogTemp, Log, TEXT("Damped Period with Air Friction: %.2f seconds"), DampedPeriod);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue,
			FString::Printf(TEXT("Theoretical Period: %.2f seconds"), TheoreticalPeriod));
	}

	FVector InitialImpulse(0.f, 6000.f, 0.f);
	MeshComp->AddImpulse(InitialImpulse);

	SetupInputBinding();
}

void APendulumActor::SetupInputBinding()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found"));
		return;
	}

	EnableInput(PlayerController);

	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	AddInstanceComponent(InputComponent);

	InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &APendulumActor::ChangeMass);
	UE_LOG(LogTemp, Log, TEXT("Input binding set for 'Y'"));
}

void APendulumActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentTime = GetWorld()->GetTimeSeconds();
	bool bCurrentlyInBalance = IsInBalance();

	if (bCurrentlyInBalance && !bWasInBalance && (CurrentTime - LastBalanceCrossTime) >= InBalanceCooldown)
	{
		BalanceCrossCount++;
		LastBalanceCrossTime = CurrentTime;

		if (BalanceCrossCount % 2 == 0)
		{
			float ActualPeriod = CurrentTime - PreviousTime;
			UE_LOG(LogTemp, Warning, TEXT("Actual Period Detected: %.2f seconds"), ActualPeriod);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red,
					FString::Printf(TEXT("Actual Period: %.2f seconds"), ActualPeriod));
			}
			if (PeriodDisplayActor)
			{
				PeriodDisplayActor->UpdatePeriodText(ActualPeriod);
			}

			PreviousTime = CurrentTime;
		}
	}
	bWasInBalance = bCurrentlyInBalance;
}

void APendulumActor::ResetValues(float mass)
{
	Mass = mass;
	UE_LOG(LogTemp, Log, TEXT("Mass set to: %.2f grams"), Mass);
	MeshComp->SetMassOverrideInKg(NAME_None, Mass / 1000.0f, true);
	MeshComp->SetWorldLocation(InitialPosition, false, nullptr, ETeleportType::TeleportPhysics);
	MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
	MeshComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

void APendulumActor::ChangeMass()
{
	Mass = bIsMassLow ? 40.0f : 10.0f;
	bIsMassLow = !bIsMassLow;
	UE_LOG(LogTemp, Log, TEXT("Mass toggled to: %.2f grams"), Mass);
	MeshComp->SetMassOverrideInKg(NAME_None, Mass / 1000.0f, true);
}

bool APendulumActor::IsIdealAngle() const
{
	FVector Direction = (GetActorLocation() - PivotLocation).GetSafeNormal();
	float AngleRadians = FMath::Acos(FVector::DotProduct(Direction, FVector::DownVector));
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	return FMath::Abs(AngleDegrees - 30.0f) <= 2.0f;
}

float APendulumActor::GetCurrentHeight() const
{
	return GetActorLocation().Z - InitialPosition.Z;
}

bool APendulumActor::IsInBalance() const
{
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorUpVector(), FVector::UpVector)));
	return FMath::Abs(Angle) < BalanceToleranceDegrees;
}
