#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorsionAxleActor.generated.h"

UCLASS()
class VRPHYSICSLAB_API ATorsionAxleActor : public AActor
{
    GENERATED_BODY()

public:
    ATorsionAxleActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    float GetCurrentAngle() const { return Angle; }
    
    UFUNCTION(BlueprintCallable , Category= "update")
    float IncrementMass();
    UFUNCTION(BlueprintCallable , Category= "update")
    float DecrementMass();
    UFUNCTION(BlueprintCallable , Category= "update")
    float IncrementDistance();
    UFUNCTION(BlueprintCallable , Category= "update")
    float DecrementDistance();
    
    UFUNCTION(BlueprintCallable , Category= "update")
    void RegenerateState();

private:
    // --- Bileşenler ---
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Rod;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* LeftWeight;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* RightWeight;

    // --- Kullanıcıdan atanabilir meshler ---
    UPROPERTY(EditAnywhere, Category = "Visual")
    UStaticMesh* RodMesh;

    UPROPERTY(EditAnywhere, Category = "Visual")
    UStaticMesh* WeightMesh;

    // --- Deneysel değişkenler (kullanıcı tarafından atanır) ---
    float massCount = 0;

    float distanceCount = 0;
    // Her bir ağırlığın gram cinsinden kütlesi
    UPROPERTY(EditAnywhere, Category = "Experiment Settings")
    float Mass = 150.0f; // gram

    // Merkezden uzaklık (cm)
    UPROPERTY(EditAnywhere, Category = "Experiment Settings")
    float DistanceFromCenter = 10.0f; // cm

    // Geri çağırıcı tork sabiti (Nm/rad)
    UPROPERTY(EditAnywhere, Category = "Experiment Settings")
    float RestoringTorque = 0.172f; // önceki 0.05 yerine

    // Yalnızca çubuğun periyodu (s)
    UPROPERTY(EditAnywhere, Category = "Experiment Settings")
    float T0 = 0.2f;

    // Hava sürtünmesi katsayısı (birimsiz)
    UPROPERTY(EditAnywhere, Category = "Experiment Settings")
    float AirFrictionCoefficient = 0.02f;

    // Başlangıç açısı (derece cinsinden)
    UPROPERTY(EditAnywhere, Category = "Experiment Settings")
    float Angle = 60.0f;

    // --- İçsel simülasyon değişkenleri ---
    float AngularVelocity = 0.0f;
    float TimeAccumulator = 0.0f;
    float CurrentPeriod = 1.0f;
};