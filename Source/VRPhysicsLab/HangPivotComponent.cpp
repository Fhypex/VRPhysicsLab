#include "HangPivotComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"  // Needed for debug drawing

UHangPivotComponent::UHangPivotComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHangPivotComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        InitialZ = Owner->GetActorLocation().Z;
    }
    SetupInputBinding();

    UE_LOG(LogTemp, Log, TEXT("HangPivotComponent BeginPlay called"));
}

void UHangPivotComponent::SetupInputBinding()
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("HangPivotComponent: Owner not found"));
        return;
    }

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("HangPivotComponent: PlayerController not found"));
        return;
    }
    Owner->EnableInput(PlayerController);
    InputComponent = Owner->FindComponentByClass<UInputComponent>();
    if (!InputComponent)
    {
        InputComponent = NewObject<UInputComponent>(Owner);
        InputComponent->RegisterComponent();
        Owner->AddInstanceComponent(InputComponent);
        UE_LOG(LogTemp, Log, TEXT("HangPivotComponent: Created new InputComponent"));
    }

    InputComponent->BindKey(EKeys::X, IE_Pressed, this, &UHangPivotComponent::MoveActor);

    Owner->EnableInput(PlayerController);

    UE_LOG(LogTemp, Log, TEXT("HangPivotComponent: Input binding set for key 'X'"));
}

void UHangPivotComponent::MoveActor()
{
    UE_LOG(LogTemp, Warning, TEXT("MoveActor called!"));

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("HangPivotComponent: Owner not found in MoveActor"));
        return;
    }

    UPhysicsConstraintComponent* FoundConstraint = Owner->FindComponentByClass<UPhysicsConstraintComponent>();
    UPrimitiveComponent* Component1 = nullptr;
    UPrimitiveComponent* Component2 = nullptr;
    FName Bone1, Bone2;

    if (FoundConstraint)
    {
        FoundConstraint->GetConstrainedComponents(Component1, Bone1, Component2, Bone2);

        FoundConstraint->BreakConstraint();
        UE_LOG(LogTemp, Log, TEXT("HangPivotComponent: Found and broke constraint."));

        if (Component2)
        {
            Component2->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            UE_LOG(LogTemp, Log, TEXT("HangPivotComponent: Detached constrained component."));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("HangPivotComponent: Constrained component was null."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HangPivotComponent: No PhysicsConstraintComponent found."));
    }

    // Calculate movement based on MoveState
    FVector OldLocation = Owner->GetActorLocation();
    FVector NewLocation = OldLocation;

    const float StepSize = 13.2f;
    float displayHeight = 80.0f;

    if (MoveState < 3)
    {
        // Move down by 13 units on states 0,1,2
        displayHeight -= (MoveState + 1) * 20.0f; // 1st = 60, 2nd = 40, 3rd = 20
        NewLocation.Z -= StepSize;
        UE_LOG(LogTemp, Log, TEXT("MoveState %d: Moving down by %f"), MoveState, StepSize);
    }
    else if (MoveState == 3)
    {
        // On 4th state, move back up by 39 units
         displayHeight = 80.0f; // reset to 80
        NewLocation.Z += StepSize * 3;
        UE_LOG(LogTemp, Log, TEXT("MoveState 3: Moving up by %f"), StepSize);
    }
    UTextRenderComponent* TextComponent = Owner->FindComponentByClass<UTextRenderComponent>();
    if (TextComponent)
    {
        TextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        FString HeightString = FString::Printf(TEXT("Length: %.0f cm"), displayHeight);
        TextComponent->SetText(FText::FromString(HeightString));
        UE_LOG(LogTemp, Log, TEXT("TextRenderComponent updated with: %s"), *HeightString);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No TextRenderComponent found on Owner."));
    }
    Owner->SetActorLocation(NewLocation);
    
    UE_LOG(LogTemp, Log, TEXT("HangPivotComponent moved actor from %s to %s"), *OldLocation.ToString(), *NewLocation.ToString());

    // Reattach Component2 and restore constraint
    if (FoundConstraint && Component1 && Component2)
    {
        Component2->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
        UE_LOG(LogTemp, Log, TEXT("HangPivotComponent: Reattached constrained component."));

        FoundConstraint->SetConstrainedComponents(Component1, NAME_None, Component2, NAME_None);
        UE_LOG(LogTemp, Log, TEXT("HangPivotComponent: Constraint re-applied."));
    }

    // Update state for next call
    MoveState = (MoveState + 1) % 4;
}

void UHangPivotComponent::ResetValues(float LengthCm)
{
    UE_LOG(LogTemp, Warning, TEXT("ResetValues called with desired length: %.2f cm"), LengthCm);

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("HangPivotComponent: Owner not found in ResetValues"));
        return;
    }

    UPhysicsConstraintComponent* FoundConstraint = Owner->FindComponentByClass<UPhysicsConstraintComponent>();
    UPrimitiveComponent* Component1 = nullptr;
    UPrimitiveComponent* Component2 = nullptr;
    FName Bone1, Bone2;

    if (FoundConstraint)
    {
        FoundConstraint->GetConstrainedComponents(Component1, Bone1, Component2, Bone2);
        FoundConstraint->BreakConstraint();

        if (Component2)
        {
            Component2->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        }
    }

    // Move actor based on the given length
    FVector CurrentLocation = Owner->GetActorLocation();
    FVector NewLocation = CurrentLocation;
    NewLocation.Z = InitialZ + ((LengthCm - 80.0f) / 1.51f);
    Owner->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

    // Reattach and reapply constraint
    if (FoundConstraint && Component1 && Component2)
    {
        Component2->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
        FoundConstraint->SetConstrainedComponents(Component1, NAME_None, Component2, NAME_None);
    }

    UE_LOG(LogTemp, Log, TEXT("HangPivotComponent: Actor moved to Z: %.2f for length %.2f cm"), NewLocation.Z, LengthCm);
}



