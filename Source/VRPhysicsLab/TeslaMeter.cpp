#include "TeslaMeter.h"
#include "PowerSupply.h"
#include "SelenoidActor.h"
#include "Components/StaticMeshComponent.h"

// Sabitler
const float MU_0 = 4 * PI * 1e-7f;  // Manyetik alan sabiti (T·m/A)
const float MagneticCalibrationFactor = 0.992f;
const float ResistancePerMeter = 0.0118f;
const float VoltageCalibrationFactor = 1.15f; // 🔧 Voltaj kalibrasyon faktörü eklendi

ATeslaMeter::ATeslaMeter()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = Root;

    MeterBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeterBody"));
    MeterBody->SetupAttachment(Root);

    LedDisplay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LedDisplay"));
    LedDisplay->SetupAttachment(Root);

    MilliTeslaDegeri = 0.0f;
}

void ATeslaMeter::BeginPlay()
{
    Super::BeginPlay();
    GenerateAll();
}

void ATeslaMeter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // UE_LOG(LogTemp, Warning, TEXT("B = %.2f mT, V = %.1f V"), MilliTeslaDegeri, Power->Voltage);
}

float ATeslaMeter::GetMagneticField(){
    return MilliTeslaDegeri;
}

void ATeslaMeter::GenerateAll(){
    APowerSupplyActor* Power = Cast<APowerSupplyActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APowerSupplyActor::StaticClass()));
    ASelenoidActor* Selenoid = Cast<ASelenoidActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASelenoidActor::StaticClass()));

    if (!Power || !Selenoid)
    {
        UE_LOG(LogTemp, Error, TEXT("Power Supply or Solenoid not found!"));
        return;
    }

    float I = Power->Current;
    float L = Selenoid->Length / 100.0f;
    float R = Selenoid->Radius / 100.0f;
    float x = Selenoid->Distance / 100.0f;
    int32 N = Selenoid->NumTurns;

    // Biot-Savart manyetik alan hesabı
    float z1 = x - (L / 2.0f);
    float z2 = x + (L / 2.0f);

    float B_Tesla = (MU_0 * N * I) / (2.0f * L) *
        (z2 / FMath::Sqrt(R * R + z2 * z2) - z1 / FMath::Sqrt(R * R + z1 * z1));

    // TeslaMeter değeri (x.xx formatında)
    MilliTeslaDegeri = FMath::RoundToFloat(B_Tesla * 1000.0f * MagneticCalibrationFactor * 100.0f) / 100.0f;

    // Tel uzunluğu ve voltaj hesabı (x.x formatında)
    float WireLength = PI * R * N * 2.0f;
    float EffectiveResistance = ResistancePerMeter * WireLength;
    float RawVoltage = EffectiveResistance * I * VoltageCalibrationFactor;
    Power->Voltage = FMath::RoundToFloat(RawVoltage * 10.0f) / 10.0f;
    
    Selenoid->GenerateSelenoid();
}



/*
 
 ----------------------------
 
 * 1
 
 Current -> 4
 Length -> 40
 NumTurns -> 120
 Radius -> 6
 Distance -> 0
 
 Beklenen (Yaklaşık) :
 Voltage -> 2.3
 TeslaMeterDegeri -> 1.54
 
 Output:
 Voltage -> 2.135
 TeslaMeterDegeri -> 1.433
 
 ----------------------------
 
 * 2
 
 Current -> 6
 Length -> 40
 NumTurns -> 120
 Radius -> 4.5
 Distance -> 0
 
 Beklenen (Yaklaşık) :
 Voltage -> 2.8
 TeslaMeterDegeri -> 2.26
 
 Output:
 Voltage -> 2.402
 TeslaMeterDegeri -> 2.189
 
 ----------------------------
 
 * 3
 
 Current -> 8
 Length -> 40
 NumTurns -> 120
 Radius -> 4.5
 Distance -> 0
 
 Beklenen (Yaklaşık) :
 Voltage -> 3.7
 TeslaMeterDegeri -> 3
 
 Output:
 Voltage -> 3.203
 TeslaMeterDegeri -> 2.919
 
 ----------------------------

 * 4
 
 Current -> 10
 Length -> 24
 NumTurns -> 30
 Radius -> 4
 Distance -> 0
 
 Beklenen (Yaklaşık) :
 Voltage -> 1.1
 TeslaMeterDegeri -> 1.52-1.53
 
 Output:
 Voltage -> 0.89
 TeslaMeterDegeri -> 1.478
 
 ----------------------------
 
 * 5
 
 Current -> 10
 Length -> 12
 NumTurns -> 30
 Radius -> 4
 Distance -> 0
 
 Beklenen (Yaklaşık) :
 Voltage -> 1.3 - 1.4
 TeslaMeterDegeri -> 2.62-2.63
 
 Output:
 Voltage -> 0.89
 TeslaMeterDegeri -> 2.593
 
 ----------------------------
 
 * 6
 
 Current -> 10
 Length -> 6
 NumTurns -> 30
 Radius -> 4
 Distance -> 0
 
 Beklenen (Yaklaşık) :
 Voltage -> 1.1
 TeslaMeterDegeri -> 3.52 - 3.53 - 3.54
 
 Output:
 Voltage -> 0.905
 TeslaMeterDegeri -> 3.74
 
 ----------------------------
 
 * 7
 
 Current -> 12
 Length -> 40
 NumTurns -> 120
 Radius -> 4.5
 Distance -> 0
 
 Beklenen (Yaklaşık) :
 Voltage -> 5.6
 TeslaMeterDegeri -> 4.30
 
 Output:
 Voltage -> 4.804
 TeslaMeterDegeri -> 4.378
 
 ----------------------------
 
 * 8
 
 Current -> 12
 Length -> 40
 NumTurns -> 120
 Radius -> 4.5
 Distance -> 8
 
 Beklenen (Yaklaşık) :
 Voltage -> 5.6
 TeslaMeterDegeri -> 4.20
 
 Output:
 Voltage -> 4.804
 TeslaMeterDegeri -> 4.316
 
 ----------------------------
 
 * 9
 
 Current -> 12
 Length -> 40
 NumTurns -> 120
 Radius -> 4.5
 Distance -> 12
 
 Beklenen (Yaklaşık) :
 Voltage -> 5.6
 TeslaMeterDegeri -> 4.00
 
 Output:
 Voltage -> 4.804
 TeslaMeterDegeri -> 4.178
 
 ----------------------------
 
 * 10
 
 Current -> 12
 Length -> 40
 NumTurns -> 120
 Radius -> 4.5
 Distance -> 16
 
 Beklenen (Yaklaşık) :
 Voltage -> 5.6
 TeslaMeterDegeri -> 3.55
 
 Output:
 Voltage -> 4.804
 TeslaMeterDegeri -> 3.717
 
 ----------------------------
 
 * 11
 
 Current -> 12
 Length -> 40
 NumTurns -> 120
 Radius -> 4.5
 Distance -> 20
 
 Beklenen (Yaklaşık) :
 Voltage -> 5.6
 TeslaMeterDegeri -> 2.25
 
 Output:
 Voltage -> 4.804
 TeslaMeterDegeri -> 2.23
 
 ----------------------------
 
 * 12
 
 Current -> 12
 Length -> 40
 NumTurns -> 120
 Radius -> 6
 Distance -> 0
 
 Beklenen (Yaklaşık) :
 Voltage -> 6.4
 TeslaMeterDegeri -> 4.4
 
 Output:
 Voltage -> 6.406
 TeslaMeterDegeri -> 4.298
 
 ----------------------------
 
 * 13
 
 Current -> 12
 Length -> 40
 NumTurns -> 120
 Radius -> 6
 Distance -> 12
 
 Beklenen (Yaklaşık) :
 Voltage -> 6.4
 TeslaMeterDegeri -> 4.0
 
 Output:
 Voltage -> 6.406
 TeslaMeterDegeri -> 4
 
 ----------------------------
 
 * 14
 
 Current -> 12
 Length -> 40
 NumTurns -> 120
 Radius -> 6
 Distance -> 16
 
 Beklenen (Yaklaşık) :
 Voltage -> 6.4
 TeslaMeterDegeri -> 3.5
 
 Output:
 Voltage -> 6.406
 TeslaMeterDegeri -> 3.458
 
 ----------------------------

*/