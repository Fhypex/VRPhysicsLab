#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerSupply.generated.h"

UCLASS()
class VRPHYSICSLAB_API APowerSupplyActor : public AActor
{
    GENERATED_BODY()

public:
    APowerSupplyActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Supply")
    float Voltage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Supply")
    float Current;
    
    UFUNCTION(BlueprintCallable , Category= "update")
    float IncrementCurrent();
    UFUNCTION(BlueprintCallable , Category= "update")
    float DecrementCurrent();
};