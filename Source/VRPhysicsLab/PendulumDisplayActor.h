#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/SceneComponent.h"
#include "PendulumDisplayActor.generated.h"

UCLASS()
class VRPHYSICSLAB_API APendulumDisplayActor : public AActor
{
    GENERATED_BODY()

public:	
    APendulumDisplayActor();
    UFUNCTION(BlueprintCallable , Category= "reset")
    void CallBeginPlay();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UTextRenderComponent* TextComponent;
    UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
    void UpdatePeriodText(float Period);
protected:
    virtual void BeginPlay() override;
};
