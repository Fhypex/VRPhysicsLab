#include "FallExperimentManager.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"

AFallExperimentManager::AFallExperimentManager()
{
    PrimaryActorTick.bCanEverTick = false;

    // Bu actor�un Player0�dan otomatik input almas�
    AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AFallExperimentManager::BeginPlay()
{
    Super::BeginPlay();

    // 1) EnableInput �a��rarak kesin InputComponent olu�tural�m
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        EnableInput(PC);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ExperimentManager: PlayerController bulunamad�!"));
    }

    // 2) InputComponent var m�, hemen bind edelim
    if (InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("ExperimentManager: InputComponent haz�r, 'Release' bind ediliyor"));
        // **Action Mapping** ismi proje ayarlar�nda birebir "Release" olmal�!
        InputComponent->BindAction(TEXT("Release"), IE_Pressed, this, &AFallExperimentManager::HandleReleaseInput);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ExperimentManager: H�L� InputComponent yok!"));
    }

    // 3) Sens�r overlap event�ini ba�la
    if (FallSensor)
    {
        FallSensor->OnFallDetected.AddDynamic(this, &AFallExperimentManager::HandleFallDetected);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ExperimentManager: FallSensor atanmam��!"));
    }

    // 4) Ba�lang�� zaman ve y�ksekli�ini al
    StartTime = GetWorld()->GetTimeSeconds();
    StartHeight = FallingObject ? FallingObject->GetActorLocation().Z * 0.01f : 0.f;

    // 5) K�tleyi oku (physics sim�lasyonu a��lmasa da GetMass() �al��mal�)
    float Mass = 0.f;
    if (FallingObject && FallingObject->Mesh)
    {
        Mass = FallingObject->Mesh->GetMass();
    }

    // 6) Ba�lang�� bilgilerini ekrana bas
    FString Intro = FString::Printf(TEXT("Start Height: %.2f m   Mass: %.2f kg"), StartHeight, Mass);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Intro);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, Intro);
    }
}

void AFallExperimentManager::HandleReleaseInput()
{
    UE_LOG(LogTemp, Warning, TEXT(">>> HandleReleaseInput called!"));

    if (FallingObject)
    {
        FallingObject->Release();

        // Resetle ki tekrar �l��m yapabilelim
        StartTime = GetWorld()->GetTimeSeconds();
        StartHeight = FallingObject->GetActorLocation().Z * 0.01f;
        bHasTriggered = false;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HandleReleaseInput: FallingObject null!"));
    }
}

void AFallExperimentManager::HandleFallDetected()
{
    if (bHasTriggered) return;
    bHasTriggered = true;

    float EndTime = GetWorld()->GetTimeSeconds();
    float Duration = EndTime - StartTime;
    float EndZ = FallingObject ? FallingObject->GetActorLocation().Z * 0.01f : 0.f;
    float HeightDiff = StartHeight - EndZ;

    float g = Duration > KINDA_SMALL_NUMBER
        ? 2.f * HeightDiff / (Duration * Duration)
        : 0.f;

    FString Result = FString::Printf(
        TEXT("Fall Time: %.3f s   g: %.2f m/s2"),
        Duration, g);

    UE_LOG(LogTemp, Warning, TEXT("%s"), *Result);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, Result);
    }
}
