#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CurveBullet.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class WANTEDFPS_API ACurveBullet : public AActor
{
	GENERATED_BODY()

public:
	ACurveBullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	float Speed = 3000.0f;

	FVector Direction;
};