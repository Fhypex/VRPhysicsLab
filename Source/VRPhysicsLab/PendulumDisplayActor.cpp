// PendulumDisplayActorBP.cpp

#include "PendulumDisplayActor.h"
#include "PendulumBallComponent.h"
#include "Components/TextRenderComponent.h"

APendulumDisplayActor::APendulumDisplayActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // Create and attach the text component to the root
    TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
    TextComponent->SetupAttachment(Root);
    TextComponent->SetHorizontalAlignment(EHTA_Center);
    TextComponent->SetWorldSize(9.0f);
    TextComponent->SetText(FText::FromString(TEXT("Waiting...")));
    TextComponent->SetVisibility(true);
    TextComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f)); // Face camera
}

void APendulumDisplayActor::UpdatePeriodText(float Period)
{
    FString TextToShow = FString::Printf(TEXT("%.2f s"), Period);
    TextComponent->SetText(FText::FromString(TextToShow));
}

void APendulumDisplayActor::BeginPlay(){
    Super::BeginPlay();
}

void APendulumDisplayActor::CallBeginPlay(){
    BeginPlay();
}
