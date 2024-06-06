// 2024  Sebastian Posada


#include "Actors/SP_PointsActor.h"
#include "Components/SP_AttributeComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASP_PointsActor::ASP_PointsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* MyRootComponent = CreateDefaultSubobject<USceneComponent>("MyRootComponent");
	RootComponent = MyRootComponent;
	
	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>("SphereCollisionComp");
	SphereCollisionComp -> SetSphereRadius(100.0f, true);
	SphereCollisionComp -> SetupAttachment(MyRootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp -> SetupAttachment(SphereCollisionComp);
}

void ASP_PointsActor::BeginPlay()
{
	Super::BeginPlay();
	SphereCollisionComp -> OnComponentBeginOverlap.AddDynamic(this, &ASP_PointsActor::OnBeginOverlap);
}

// On Begin Overlap we check if Actor is valid and has Attribute comp, if yes, updates Score and destroy this actor
void ASP_PointsActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsValid(OtherActor))
	{
		FVector Location = GetActorLocation();
		FRotator Rotation = GetActorRotation();
		FVector Scale = GetActorScale();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, Location, Rotation, Scale, true, EPSCPoolMethod::None, true);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AudioComp, Location, Rotation, 1, 1, 0, nullptr, nullptr,true);

		USP_AttributeComponent* ActorAttributeComp = OtherActor->GetComponentByClass<USP_AttributeComponent>();

		if(IsValid(ActorAttributeComp))
		{
			ActorAttributeComp->ApplyScoreChange(PointsToAward);
		}

		this->Destroy();
	}
}

