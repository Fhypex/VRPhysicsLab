#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerSupply.h"
#include "SelenoidActor.h"
#include "Kismet/GameplayStatics.h"
#include "TeslaMeter.generated.h"

UCLASS()
class VRPHYSICSLAB_API ATeslaMeter : public AActor
{
    GENERATED_BODY()

public:
    ATeslaMeter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Ayarlanabilir mT değeri (milliTesla)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaMeter")
    float MilliTeslaDegeri;

    // LED Göstergesi için referans
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeterBody;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* LedDisplay;

    UFUNCTION(BlueprintCallable , Category= "reset")
    float GetMagneticField();
    
    
    UFUNCTION(BlueprintCallable , Category= "update")
    void GenerateAll();

private:
    UPROPERTY()
    USceneComponent* Root;
};