#include "FallingObject.h"
#include "Components/StaticMeshComponent.h"

AFallingObject::AFallingObject()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // start with physics off; Release() will turn it on
    Mesh->SetSimulatePhysics(false);

  
}

void AFallingObject::BeginPlay()
{
    Super::BeginPlay();
}

void AFallingObject::Release()
{
    if (!Mesh) return;

    Mesh->SetSimulatePhysics(true);
    Mesh->SetEnableGravity(true);
}

void AFallingObject::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bUseCustomDrag || !Mesh->IsSimulatingPhysics())
    {
        return;
    }

    // 1) Get velocity in cm/s ? convert to m/s
    FVector V_cm = Mesh->GetPhysicsLinearVelocity();
    FVector V = V_cm * 0.01f;
    float Speed = V.Size();
    if (Speed < KINDA_SMALL_NUMBER)
    {
        return;
    }

    // 2) Approximate sphere radius from bounds (cm ? m)
    FVector Origin, BoxExt;
    Mesh->GetLocalBounds(Origin, BoxExt);
    float Radius_cm = BoxExt.GetMax() * Mesh->GetComponentScale().X;
    float Radius = Radius_cm * 0.01f;

    // 3) Cross-sectional area (m²)
    const float Area = PI * Radius * Radius;

    // 4) Quadratic drag magnitude: ½·?·Cd·A·v²
    const float Rho = 1.225f;  // air density kg/m³
    float DragMag = 0.5f * Rho * DragCoefficient * Area * Speed * Speed;
    FVector Drag = -V.GetSafeNormal() * DragMag;

    // 5) Convert N to Unreal units (1 N = 100 kg·cm/s²)
    FVector DragForce = Drag * 100.0f;

    // 6) Apply force
    Mesh->AddForce(DragForce);
}
