#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelenoidActor.generated.h"

UCLASS()
class VRPHYSICSLAB_API ASelenoidActor : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    ASelenoidActor();

protected:
    // OnConstruction methodu, aktör sahnede oluşturulduğunda çalışır.
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

public:
    // Ayarlanabilir parametreler. EditAnywhere: Editörde görünmesini sağlar, BlueprintReadWrite: Blueprint'lerde okunabilir/yazılabilir.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selenoid")
    float Length = 10.0f;  // Uzunluk parametresi

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selenoid")
    int32 NumTurns = 10;    // Sarım sayısı

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selenoid")
    float Radius = 4.0f;   // Yarıçap
    
    // Uzaklasma değiskeni
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selenoid")
    float Distance = 0.0f;

    // Görünürlük için kullanılan coil mesh (tel şeklindeki görünüm)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selenoid")
    UStaticMesh* CoilMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selenoid")
    UMaterialInterface* CoilMaterial;

    // Spline oluşturulmasını sağlayan bileşen
    UPROPERTY(VisibleAnywhere)
    class USplineComponent* SplineComponent;
    
    
    UFUNCTION(BlueprintCallable , Category= "update")
    int32 IncrementNumTurns();
    UFUNCTION(BlueprintCallable , Category= "update")
    float IncrementLength();
    UFUNCTION(BlueprintCallable , Category= "update")
    float IncrementRadius();
    UFUNCTION(BlueprintCallable , Category= "update")
    float IncrementDistance();
    
    UFUNCTION(BlueprintCallable , Category= "update")
    int32 DecrementNumTurns();
    UFUNCTION(BlueprintCallable , Category= "update")
    float DecrementLength();
    UFUNCTION(BlueprintCallable , Category= "update")
    float DecrementRadius();
    UFUNCTION(BlueprintCallable , Category= "update")
    float DecrementDistance();
    
    UFUNCTION(BlueprintCallable , Category= "update")
    void GenerateSelenoid();
    

    int getNumTurns();

    float getRadius();

    float getLength();


private:
    // Spline mesh bileşenlerinin dizisi
    TArray<class USplineMeshComponent*> MeshComponents;


   USplineMeshComponent* MeshComp = nullptr;
   
    
};