// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "pendulum_display.generated.h"

UCLASS()
class VRPHYSICSLAB_API Apendulum_display : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Apendulum_display();

	// Geri çağırıcı tork sabiti (Nm/rad)
    UPROPERTY(EditAnywhere, Category = "value")
    float Length = 80.0f;
	
	UFUNCTION(BlueprintCallable , Category= "update")
    float Increment();

	UFUNCTION(BlueprintCallable , Category= "update")
    float GetLength();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
