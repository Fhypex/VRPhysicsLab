#include "PendulumBallComponent.h"
#include "HAL/IConsoleManager.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h" // Bu başlığı eklemeyi unutma
#include "CableComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPendulumBallComponent::UPendulumBallComponent()
{

	PivotLocation = InitialPosition + FVector(0.0f, 0.0f, 60.6f); // Adjust height
	PrimaryComponentTick.bCanEverTick = true;
	Mass = 1.0f; // Adjust to match your actual object mass
	Gravity = 980.0f;
	BalanceToleranceDegrees = 1.0f;
	LastBalanceTime = 0.0f;
	PreviousTime = 0;
	EnergyLogCooldown = 1.25f; // Log energy once every 1 second
	LastEnergyLogTime = -EnergyLogCooldown;
	LastPeriodLogTime = -EnergyLogCooldown;// allows immediate first log
	bIsMassLow = true;
}

// katsayı 1.51
void UPendulumBallComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialPosition = GetOwner()->GetActorLocation();
	 //UE_LOG(LogTemp, Warning, TEXT("INITIAL POS: %.2f") , InitialPosition);
	TextRenderComp = GetOwner()->FindComponentByClass<UTextRenderComponent>();
    if (!TextRenderComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("TextRenderComponent not found on the actor!"));
    }
	SetupInputBinding();
	// Get StaticMeshComponent and configure
	UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	if (MeshComp)
	{
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetEnableGravity(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
		MeshComp->SetMobility(EComponentMobility::Movable);
		GetOwner()->Tags.Add("Grabbable");
		UE_LOG(LogTemp, Log, TEXT("StaticMeshComponent is now grabbable!"));
		FVector InitialImpulse = FVector(6000.f, 0.f,0.f);// Yönü sağa doğru (Y ekseni), büyüklüğü ayarla
        MeshComp->AddImpulse(InitialImpulse);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Root is not a StaticMeshComponent!"));
	}
	if (TextRenderComp)
	{
		TextRenderComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);	
		FString TextToShow = FString::Printf(TEXT("Period: ???"));	
		TextRenderComp->SetText(FText::FromString(TextToShow));		
	}
	TheoreticalPeriod = 2 * PI * FMath::Sqrt(CableLength / Gravity);
	UE_LOG(LogTemp, Log, TEXT("Theoretical Period: %.2f seconds"), TheoreticalPeriod);
	float DampingCoefficient = 0.2f; // air friction constant (adjust based on realism)

	float Gamma = DampingCoefficient / Mass;
	float DampedPeriod = TheoreticalPeriod * (1 + (Gamma * Gamma) / 16.0f);

	UE_LOG(LogTemp, Log, TEXT("Damped Period with Air Friction: %.2f seconds"), DampedPeriod);
	
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue,
		FString::Printf(TEXT("Theoretical Period: %.2f seconds"), TheoreticalPeriod));

	
}

void UPendulumBallComponent::SetupInputBinding()
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("PendulumBallComponent: Owner not found"));
        return;
    }

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PendulumBallComponent: PlayerController not found"));
        return;
    }

    Owner->EnableInput(PlayerController);

    InputComponent = Owner->FindComponentByClass<UInputComponent>();
    if (!InputComponent)
    {
        InputComponent = NewObject<UInputComponent>(Owner);
        InputComponent->RegisterComponent();
        Owner->AddInstanceComponent(InputComponent);
        UE_LOG(LogTemp, Log, TEXT("PendulumBallComponent: Created new InputComponent"));
    }

    if (InputComponent)
    {
        InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &UPendulumBallComponent::ChangeMass);
		InputComponent->BindKey(EKeys::V, IE_Pressed, this, &UPendulumBallComponent::ResetBallPosition);
		InputComponent->BindKey(EKeys::C, IE_Pressed, this, &UPendulumBallComponent::ApplyImpulse);
        UE_LOG(LogTemp, Log, TEXT("PendulumBallComponent: Input binding set for key 'Y'"));
    }
}

// Called every frame
void UPendulumBallComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	

	bool bCurrentlyInBalance = IsInBalance();
	
	if (bCurrentlyInBalance && !bWasInBalance && (CurrentTime - LastBalanceCrossTime) >= InBalanceCooldown)
	{
		BalanceCrossCount++;
		LastBalanceCrossTime = CurrentTime;

		if (BalanceCrossCount % 2 == 0) // Full period: 2 crossings
		{			float ActualPeriod = CurrentTime - PreviousTime;
		

			
			if (PeriodDisplayActor)
			{
				float DisplayHeight = (ActualPeriod < 2.5f) ? ActualPeriod : ActualPeriod / 2;
							
				PeriodDisplayActor->UpdatePeriodText(DisplayHeight);
			}

			PreviousTime = CurrentTime;
		}
	}
	bWasInBalance = bCurrentlyInBalance;


	if (IsIdealAngle())
	{
		//UE_LOG(LogTemp, Log, TEXT("Within ideal angle debug"));
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (Mesh && CurrentTime - LastPeriodLogTime >= EnergyLogCooldown)
		{
			UE_LOG(LogTemp, Log, TEXT("Ball is within ideal angle."));

		}
	}
	
}

void UPendulumBallComponent::ResetValues(float mass)
{
    Mass = mass;
    UE_LOG(LogTemp, Log, TEXT("Mass toggled to: %.2f grams"), Mass);

    UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    if (MeshComp)
    {
        // Set the new mass override (kg), converting grams to kg
        MeshComp->SetMassOverrideInKg(NAME_None, Mass / 1000.0f, true);

        // Reset position
        MeshComp->SetWorldLocation(InitialPosition, false, nullptr, ETeleportType::TeleportPhysics);

        // Remove all linear and angular velocity
        MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
        MeshComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

        // Clear any forces/torques if you've applied them manually
    }
}

void UPendulumBallComponent::ChangeMass()
{
	
    if (bIsMassLow)
    {
        Mass = 40.0f;  // High mass in grams
    }
    else
    {
        Mass = 10.0f;  // Low mass in grams
    }

    bIsMassLow = !bIsMassLow;

    UE_LOG(LogTemp, Log, TEXT("Mass toggled to: %.2f grams"), Mass);

    UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    if (MeshComp)
    {
        // Set the new mass override (kg), converting grams to kg
        MeshComp->SetMassOverrideInKg(NAME_None, Mass / 1000.0f, true);
    }
}



bool UPendulumBallComponent::IsIdealAngle() const
{
	FVector CurrentPosition = GetOwner()->GetActorLocation();
	FVector Direction = (CurrentPosition - PivotLocation).GetSafeNormal();

	float AngleRadians = FMath::Acos(FVector::DotProduct(Direction, FVector::DownVector));
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	float IdealAngle = 30.0f;
	float AngleTolerance = 2.0f;

	return FMath::Abs(AngleDegrees - IdealAngle) <= AngleTolerance;
}

float UPendulumBallComponent::GetCurrentHeight() const
{
	FVector CurrentPosition = GetOwner()->GetActorLocation();
	return CurrentPosition.Z - InitialPosition.Z;
}


bool UPendulumBallComponent::IsInBalance() const
{
	FVector UpVector = GetOwner()->GetActorUpVector();
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(UpVector, FVector::UpVector)));

	return FMath::Abs(Angle) < BalanceToleranceDegrees;
}

void UPendulumBallComponent::ResetBallPosition()
{
    UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    if (MeshComp)
    {
        
        MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
        MeshComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		MeshComp->SetWorldLocation(InitialPosition, false, nullptr, ETeleportType::TeleportPhysics);
		MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
        MeshComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		MeshComp->SetWorldLocation(InitialPosition, false, nullptr, ETeleportType::TeleportPhysics);
		MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
        MeshComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		MeshComp->SetWorldLocation(InitialPosition, false, nullptr, ETeleportType::TeleportPhysics);
        UE_LOG(LogTemp, Log, TEXT("Ball reset to initial position."));
    }
}



void UPendulumBallComponent::ApplyImpulse()
{
    UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    if (MeshComp)
    {
        FVector InitialImpulse = FVector(6000.f, 0.f,0.f);
		MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
        MeshComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
        MeshComp->AddImpulse(InitialImpulse);
        UE_LOG(LogTemp, Log, TEXT("Impulse applied to ball."));
    }
}
