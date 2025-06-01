#include "PeriodDisplayActor.h"
#include "TorsionAxleActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

APeriodDisplayActor::APeriodDisplayActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
    TextComponent->SetupAttachment(Root);
    TextComponent->SetHorizontalAlignment(EHTA_Center);
    TextComponent->SetWorldSize(9.0f);  // Daha büyük yazı
    TextComponent->SetText(FText::FromString(TEXT("--")));
    TextComponent->SetVisibility(true);
    TextComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f)); // Kameraya dönük hale getir

    bWasPositiveAngle = true;
    LastCrossingTime = 0.0f;
    PreviousCrossingTime = 0.0f;
    bFirstPass = true;
}


void APeriodDisplayActor::BeginPlay()
{
    Super::BeginPlay();

    LastCrossingTime = UGameplayStatics::GetTimeSeconds(GetWorld());
    TextComponent->SetText(FText::FromString(TEXT("Waiting...")));

    
}

void APeriodDisplayActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TrackedAxle)
    {
        return;
    }

    float Angle = TrackedAxle->GetCurrentAngle();
    float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());

    bool bIsCurrentlyPositive = Angle >= 0.0f;

    // Denge noktasından geçiş
    if (bIsCurrentlyPositive != bWasPositiveAngle)
    {
        bWasPositiveAngle = bIsCurrentlyPositive;

        if (!bFirstPass)
        {
            float Period = CurrentTime - PreviousCrossingTime;
            FString PeriodText = FString::Printf(TEXT("%.3f s"), Period);
            TextComponent->SetText(FText::FromString(PeriodText));

            UE_LOG(LogTemp, Warning, TEXT("Measured Period: %.3f seconds"), Period);
        }
        else
        {
            bFirstPass = false;
        }

        PreviousCrossingTime = CurrentTime;
    }
}
