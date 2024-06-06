// 2024  Sebastian Posada

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SP_AttributeComponent.generated.h"

// Delegate to broadcast OnScoreChanged events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnScoreChanged, AActor*, InstigatorActor, USP_AttributeComponent*, OwningComp, float, NewScore, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SP_BGSTASK_API USP_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USP_AttributeComponent();

protected:

	UPROPERTY(BlueprintReadOnly, CAtegory = "Attributes")
	float CurrentScore = 0.0f;

public:

	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;

	UFUNCTION(BlueprintCallable)
	bool ApplyScoreChange(float Delta);

		
};
