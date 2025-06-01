// FallSensor.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "FallSensor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFallDetected);

UCLASS()
class VRPHYSICSLAB_API AFallSensor : public AActor
{
    GENERATED_BODY()

public:
    AFallSensor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fall Sensor")
    UBoxComponent* Trigger;

    UPROPERTY(BlueprintAssignable, Category = "Fall Sensor")
    FOnFallDetected OnFallDetected;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};