// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DenemeActor.generated.h"

UCLASS()
class VRPHYSICSLAB_API ADenemeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADenemeActor();
	UFUNCTION(BlueprintCallable , Category= "reset")
	void CallBeginPlayDeneme();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
