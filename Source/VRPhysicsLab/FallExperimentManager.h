#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallSensor.h"
#include "FallingObject.h"
#include "FallExperimentManager.generated.h"

UCLASS()
class VRPHYSICSLAB_API AFallExperimentManager : public AActor
{
    GENERATED_BODY()

public:
    AFallExperimentManager();

    /** Level�a yerleyece�iniz top akt�r� */
    UPROPERTY(EditAnywhere, Category = "Experiment")
    AFallingObject* FallingObject;

    /** Ground trigger akt�r� */
    UPROPERTY(EditAnywhere, Category = "Experiment")
    AFallSensor* FallSensor;

protected:
    virtual void BeginPlay() override;

    /** E tu�una bas�ld���nda �a�r�l�r */
    void HandleReleaseInput();

    /** Sensor overlap��nda �a�r�l�r */
    UFUNCTION()
    void HandleFallDetected();

    float StartTime = 0.f;
    float StartHeight = 0.f;
    bool  bHasTriggered = false;

public:
    virtual void Tick(float DeltaTime) override {}
};
