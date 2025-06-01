#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PeriodDisplayActor.generated.h"

class UTextRenderComponent;
class ATorsionAxleActor;

UCLASS()
class VRPHYSICSLAB_API APeriodDisplayActor : public AActor
{
    GENERATED_BODY()

public:
    APeriodDisplayActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ATorsionAxleActor* TrackedAxle;

    UPROPERTY(VisibleAnywhere)
    UTextRenderComponent* TextComponent;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;  

private:
    bool bWasPositiveAngle;
    float LastCrossingTime;
    float PreviousCrossingTime;
    bool bFirstPass;
};
