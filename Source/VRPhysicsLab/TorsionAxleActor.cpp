#include "TorsionAxleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Math/UnrealMathUtility.h"

ATorsionAxleActor::ATorsionAxleActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Rod = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rod"));
    Rod->SetupAttachment(Root);

    LeftWeight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWeight"));
    LeftWeight->SetupAttachment(Rod);

    RightWeight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWeight"));
    RightWeight->SetupAttachment(Rod);
}

void ATorsionAxleActor::BeginPlay()
{
    Super::BeginPlay();

    float MassInKg = Mass / 1000.0f;
    float RadiusInMeters = DistanceFromCenter / 100.0f;

    // ✅ Periyot hesapla (RestoringTorque sabit)
    float R2 = RadiusInMeters * RadiusInMeters;
    float T2 = ((8 * PI * PI * MassInKg) / RestoringTorque) * R2 + (T0 * T0);
    CurrentPeriod = FMath::Sqrt(T2);

    // ✅ Görsel hizalama (80 cm çubuk varsayımı)
    float VirtualRodLengthCM = 80.0f;
    float RodLengthUnits = Rod->Bounds.BoxExtent.X * 2.0f;
    float UnitsPerCM = RodLengthUnits / VirtualRodLengthCM;
    float DistanceInUnits = DistanceFromCenter * UnitsPerCM;

    if (LeftWeight)
    {
        FVector Loc = LeftWeight->GetRelativeLocation();
        LeftWeight->SetRelativeLocation(FVector(-DistanceInUnits * 4, Loc.Y, Loc.Z));
    }

    if (RightWeight)
    {
        FVector Loc = RightWeight->GetRelativeLocation();
        RightWeight->SetRelativeLocation(FVector(DistanceInUnits * 4, Loc.Y, Loc.Z));
    }
}

void ATorsionAxleActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeAccumulator += DeltaTime;

    float MassInKg = Mass / 1000.0f;
    float RadiusInMeters = DistanceFromCenter / 100.0f;

    
    float Inertia = 2 * MassInKg * FMath::Square(RadiusInMeters);
    float RestoringAcceleration = - (RestoringTorque / Inertia) * FMath::DegreesToRadians(Angle);

    AngularVelocity += RestoringAcceleration * DeltaTime;
    AngularVelocity -= AirFrictionCoefficient * AngularVelocity * DeltaTime;

    Angle += AngularVelocity * DeltaTime;

    if (FMath::Abs(AngularVelocity) < 0.01f && FMath::Abs(Angle) < 0.5f)
    {
        AngularVelocity = 0.0f;
        Angle = 0.0f;
    }

    FRotator NewRotation = FRotator(0, Angle, 0);
    Rod->SetRelativeRotation(NewRotation);
}



void ATorsionAxleActor::RegenerateState()
{
    float MassInKg = Mass / 1000.0f;
    float RadiusInMeters = DistanceFromCenter / 100.0f;

    // Periyot hesapla
    float R2 = RadiusInMeters * RadiusInMeters;
    float T2 = ((8 * PI * PI * MassInKg) / RestoringTorque) * R2 + (T0 * T0);
    CurrentPeriod = FMath::Sqrt(T2);

    // Görsel hizalama
    float VirtualRodLengthCM = 80.0f;
    float RodLengthUnits = Rod->Bounds.BoxExtent.X * 2.0f;
    float UnitsPerCM = RodLengthUnits / VirtualRodLengthCM;
    float DistanceInUnits = DistanceFromCenter * UnitsPerCM;

    if (LeftWeight)
    {
        FVector Loc = LeftWeight->GetRelativeLocation();
        LeftWeight->SetRelativeLocation(FVector(-DistanceInUnits * 4, Loc.Y, Loc.Z));
    }

    if (RightWeight)
    {
        FVector Loc = RightWeight->GetRelativeLocation();
        RightWeight->SetRelativeLocation(FVector(DistanceInUnits * 4, Loc.Y, Loc.Z));
    }

    // Simülasyon başlangıcını sıfırla
    Angle = 30.0f;              // Varsayılan başlangıç açısı
    AngularVelocity = 0.0f;
    TimeAccumulator = 0.0f;
}


// 50 <- +-50 -> 400
float ATorsionAxleActor::IncrementMass(){
    // Increment
    Mass += 50;
    // check if it is on bounds
    if(Mass > 400){
        Mass = 50;
    }
    return Mass;
}

float ATorsionAxleActor::DecrementMass(){
    // Decrement
    Mass -= 50;
    // check if it on bounds or not
    if(Mass < 50){
        Mass = 300;
    }
    return Mass;
}

// 5 <- +-5 -> 40
float ATorsionAxleActor::IncrementDistance(){
    // Increment
    DistanceFromCenter += 5;
    // check if it is on bounds
    if(DistanceFromCenter > 40){
        DistanceFromCenter = 5;
    }
    return DistanceFromCenter;
}

float ATorsionAxleActor::DecrementDistance(){
    // Decrement
    DistanceFromCenter -= 5;
    // check if it on bounds or not
    if(DistanceFromCenter < 5){
        DistanceFromCenter = 40;
    }
    return DistanceFromCenter;
}