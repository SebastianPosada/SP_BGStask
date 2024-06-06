// 2024  Sebastian Posada


#include "Components/SP_AttributeComponent.h"

// Sets default values for this component's properties
USP_AttributeComponent::USP_AttributeComponent()
{
	
}

bool USP_AttributeComponent::ApplyScoreChange(float Delta)
{
	float OldScore = CurrentScore;
	CurrentScore += Delta;

	float ActualDelta = CurrentScore - OldScore;
	OnScoreChanged.Broadcast(nullptr, this, CurrentScore, ActualDelta);

	return ActualDelta != 0;
}

