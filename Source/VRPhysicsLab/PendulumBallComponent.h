#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TextRenderComponent.h"
#include "PendulumDisplayActor.h"
#include "PendulumBallComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VRPHYSICSLAB_API UPendulumBallComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPendulumBallComponent();

protected:
	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(Exec)
	bool IsIdealAngle() const;
	void SetupInputBinding();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pendulum Settings")
	APendulumDisplayActor* PeriodDisplayActor;
	void ResetValues(float mass);
    void ChangeMass();
	void ResetBallPosition();
	void ApplyImpulse();
private:
	FVector InitialPosition;
	FVector PivotLocation;
	float PreviousTime;
	float TheoreticalPeriod;
	UInputComponent* InputComponent = nullptr;
	UPROPERTY()
	UTextRenderComponent* TextRenderComp = nullptr;
	UPROPERTY(EditAnywhere, Category = "Pendulum Settings")
	float Mass;
	UPROPERTY(EditAnywhere, Category = "Pendulum Settings")
	float CableLength;
	bool bIsMassLow = true; 
	float Gravity;
	float BalanceToleranceDegrees;
	float EnergyLogCooldown;
	bool bHasStarted = false;
	float LastEnergyLogTime;
	float LastPeriodLogTime;
	bool bWasInBalance = false;
	int BalanceCrossCount = 0;
	float LastBalanceCrossTime = 0.0f;
	float InBalanceCooldown = 0.2f;
	float GetCurrentHeight() const;
	bool IsInBalance() const;

	float LastBalanceTime;
};
