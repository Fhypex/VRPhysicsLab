// Fill out your copyright notice in the Description page of Project Settings.


#include "pendulum_display.h"

// Sets default values
Apendulum_display::Apendulum_display()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Apendulum_display::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Apendulum_display::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float Apendulum_display::Increment(){
	Length -= 20;
	if(Length < 20){
		Length = 80;
	}
	return Length;
}



float Apendulum_display::GetLength(){
	return Length;
}

