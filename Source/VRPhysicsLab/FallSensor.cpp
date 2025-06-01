// FallSensor.cpp
#include "FallSensor.h"

AFallSensor::AFallSensor()
{
    PrimaryActorTick.bCanEverTick = false;
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    RootComponent = Trigger;
    Trigger->SetCollisionProfileName(TEXT("Trigger"));
}

void AFallSensor::BeginPlay()
{
    Super::BeginPlay();
    if (Trigger)
    {
        Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFallSensor::OnOverlapBegin);
    }
}

void AFallSensor::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    OnFallDetected.Broadcast();
}