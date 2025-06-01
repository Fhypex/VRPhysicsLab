// FallingObject.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "FallingObject.generated.h"

UCLASS()
class VRPHYSICSLAB_API AFallingObject : public AActor
{
    GENERATED_BODY()

public:
    AFallingObject();

    /** Mesh component that we�ll toggle physics on */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    /** Call this (e.g. on E key) to start the fall */
    UFUNCTION(BlueprintCallable, Category = "Falling Object")
    void Release();

    /** Whether to apply custom quadratic drag in Tick */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag")
    bool bUseCustomDrag = true;

    /** Drag coefficient Cd (sphere ?0.47) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag", meta = (ClampMin = "0.0"))
    float DragCoefficient = 0.47f;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};