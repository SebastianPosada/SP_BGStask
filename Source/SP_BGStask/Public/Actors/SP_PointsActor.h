// 2024  Sebastian Posada

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SP_PointsActor.generated.h"

UCLASS()
class SP_BGSTASK_API ASP_PointsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASP_PointsActor();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Points)
	float PointsToAward = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
	UParticleSystem* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
	USoundBase* AudioComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Components")
	USphereComponent* SphereCollisionComp;
private:
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


};
