// PowerSupplyActor.cpp

#include "PowerSupply.h"

// Sets default values
APowerSupplyActor::APowerSupplyActor()
{
    // Enable ticking every frame
    PrimaryActorTick.bCanEverTick = true;

    // Initialize default values
    Voltage = 0.0f;
    Current = 12.0f;
}
// Called when the game starts or when spawned
void APowerSupplyActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APowerSupplyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // (Gerekirse) değerleri loglama örneği:
    // UE_LOG(LogTemp, Warning, TEXT("Voltage: %f, Current: %f"), Voltage, Current);
}



// 0 -> +-1 -> 20
float APowerSupplyActor::IncrementCurrent(){
    Current += 1;
    if(Current > 20){
        Current = 1;
    }
    return Current;
}

float APowerSupplyActor::DecrementCurrent(){
    Current -= 1;
    if(Current < 0){
        Current = 20;
    }
    return Current;
}