#include "SelenoidActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

ASelenoidActor::ASelenoidActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SelenoidSpline"));
    RootComponent = SplineComponent;
}

void ASelenoidActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void ASelenoidActor::BeginPlay()
{
    Super::BeginPlay();
    GenerateSelenoid();
}

void ASelenoidActor::GenerateSelenoid()
{
    // Temizleme
    SplineComponent->ClearSplinePoints();
      if(MeshComp){
        MeshComp = nullptr;
    }

    for (USplineMeshComponent* Comp : MeshComponents)
    {
        if (Comp)
        {
            Comp->DestroyComponent();
        }
    }
    MeshComponents.Empty();

    const int32 NumPoints = NumTurns * 40; // Daha düzgün spiral

    for (int32 i = 0; i <= NumPoints; ++i)
    {
        float t = (float)i / NumPoints;
        float angle = t * NumTurns * 2 * PI;
        float z = t * Length;
        float x = Radius * FMath::Cos(angle);
        float y = Radius * FMath::Sin(angle);
        // Spiral noktası oluştururken:
        FVector OffsetDirection = FVector(0.0f, 0.0f, (-1 * Length / 2) + Distance); // veya başka bir yön

        SplineComponent->AddSplinePoint(FVector(x, y, z) + OffsetDirection, ESplineCoordinateSpace::Local);


    }

    SplineComponent->UpdateSpline();

    if (!CoilMesh) return; // Mesh yoksa çık

    for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints() - 1; ++i)
    {
        FVector StartPos, StartTangent, EndPos, EndTangent;
        SplineComponent->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
        SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

         MeshComp = NewObject<USplineMeshComponent>(this);
        MeshComp->SetStaticMesh(CoilMesh);

        if (CoilMaterial)
        {
            MeshComp->SetMaterial(0, CoilMaterial);
        }

        MeshComp->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
        MeshComp->SetStartScale(FVector2D(0.05f, 0.05f)); // Tel gibi görünmesi için küçültülmüş çap
        MeshComp->SetEndScale(FVector2D(0.05f, 0.05f));
        MeshComp->SetForwardAxis(ESplineMeshAxis::X); // <-- TEL gibi hizalanma buradan gelir
        MeshComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f)); // Silindiri Z ekseninde 90 derece döndür
        MeshComp->SetMobility(EComponentMobility::Movable);
        MeshComp->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
        MeshComp->RegisterComponent();

        MeshComponents.Add(MeshComp);
    }
}

int ASelenoidActor::getNumTurns(){
    return NumTurns;
}

float ASelenoidActor::getRadius(){
    return Radius;
}

float ASelenoidActor::getLength(){
    return Length;
}




// 5 -> +-5 -> 120
int32 ASelenoidActor::IncrementNumTurns(){
    NumTurns += 5;
    if(NumTurns > 120){
        NumTurns = 5;
    }
    return NumTurns;
}

// 5 -> +-5 -> 50
float ASelenoidActor::IncrementLength(){
    Length += 5;
    if(Length > 50){
        Length = 5;
    }
    return Length;
}

// 0.5 -> +-5 -> 10
float ASelenoidActor::IncrementRadius(){
    Radius += 0.5;
    if(Radius > 10){
        Radius = 0.5;
    }
    return Radius;
}

// 0 -> +-2 -> 20
float ASelenoidActor::IncrementDistance(){
    Distance += 2;
    if(Distance > 20){
        Distance = 0;
    }
    return Distance;
}




int32 ASelenoidActor::DecrementNumTurns(){
    NumTurns -= 5;
    if (NumTurns < 5){
        NumTurns = 120;
    }
    return NumTurns;
}

float ASelenoidActor::DecrementLength(){
    Length -= 5;
    if (Length < 5){
        Length = 50;
    }
    return Length;
}

float ASelenoidActor::DecrementRadius(){
    Radius -= 0.5;
    if (Radius < 0.5){
        Radius = 10;
    }
    return Radius;
}

float ASelenoidActor::DecrementDistance(){
    Distance -= 2;
    if (Distance < 0){
        Distance = 20;
    }
    return Distance;
}