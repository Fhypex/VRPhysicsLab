#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TextRenderComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "HangPivotComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VRPHYSICSLAB_API UHangPivotComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHangPivotComponent();
    void ResetValues(float lengthCm);
private:
    int MoveState = 0;  // from 0 to 3 (4 states)
    float InitialZ = 0.0f;
protected:
    virtual void BeginPlay() override;
    // Function to move the owning actor
    void MoveActor();

    // Input binding function
    void SetupInputBinding();

    // Reference to the InputComponent
    UInputComponent* InputComponent;
};
