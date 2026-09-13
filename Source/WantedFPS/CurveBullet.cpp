#include "CurveBullet.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ACurveBullet::ACurveBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// “–‚½‚è”»’è
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->InitSphereRadius(5.0f);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// ’e‚ÌŒ©‚½–Ú
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere")
	);

	if (SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
		Mesh->SetRelativeScale3D(FVector(0.1f));
	}
}

void ACurveBullet::BeginPlay()
{
	Super::BeginPlay();

	// Actor‚ªŒü‚¢‚Ä‚¢‚é•ûŒü‚ğ’e‚Ìis•ûŒü‚É‚·‚é
	Direction = GetActorForwardVector();

	if (GetOwner()) {
		Collision->IgnoreActorWhenMoving(GetOwner(), true);
	}

	// 5•bŒo‚Á‚½‚ç©“®íœ
	SetLifeSpan(5.0f);
}

void ACurveBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector MoveAmount = Direction * Speed * DeltaTime;

	FHitResult Hit;

	AddActorWorldOffset(
		MoveAmount,
		true,       // Sweep‚µ‚Ä“–‚½‚è”»’è‚·‚é
		&Hit
	);

	if (Hit.bBlockingHit)
	{
		Destroy();
	}
}