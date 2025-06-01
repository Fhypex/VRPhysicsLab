#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PendulumDisplayActor.h"
#include "PendulumActor.generated.h"

UCLASS()
class VRPHYSICSLAB_API APendulumActor : public AActor
{
	GENERATED_BODY()

public:	
	APendulumActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Exec)
	bool IsIdealAngle() const;

	void SetupInputBinding();
	void ResetValues(float mass);
	void ChangeMass();

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRenderComp;

	UPROPERTY(EditAnywhere, Category = "Pendulum Settings")
	APendulumDisplayActor* PeriodDisplayActor;

	FVector InitialPosition;
	FVector PivotLocation;
	float PreviousTime;
	float TheoreticalPeriod;
	UInputComponent* InputComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pendulum Settings")
	float Mass = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Pendulum Settings")
	float CableLength = 100.0f;

	bool bIsMassLow = true;
	float Gravity = 980.0f;
	float BalanceToleranceDegrees = 1.0f;
	float EnergyLogCooldown = 1.25f;
	bool bWasInBalance = false;
	int BalanceCrossCount = 0;
	float LastBalanceCrossTime = 0.0f;
	float InBalanceCooldown = 0.2f;
	float LastEnergyLogTime = -1.25f;
	float LastPeriodLogTime = -1.25f;
	float LastBalanceTime = 0.0f;

	float GetCurrentHeight() const;
	bool IsInBalance() const;
};
