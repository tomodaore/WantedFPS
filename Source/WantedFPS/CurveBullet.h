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

	void SetCurveStrength(float InCurveStrength);

	void SetCurveDirection(float InCurveDirection);

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	float Speed = 3000.0f;

	FVector Direction;

	float CurveStrength = 0.0f;

	// カーブが完成する基準距離
	UPROPERTY(EditAnywhere, Category = "Bullet|Curve")
	float CurveRange = 3000.0f;

	// 最大チャージ時の、正面からの角度
	UPROPERTY(EditAnywhere, Category = "Bullet|Curve")
	float MaxCurveAngle = 45.0f;

	// ベジェ曲線の膨らみ具合
	UPROPERTY(EditAnywhere, Category = "Bullet|Curve")
	float CurveHandleRatio = 0.5f;

	float CurveT = 0.0f;

	bool bCurveInitialized = false;
	bool bCurveFinished = false;

	FVector CurveP0;
	FVector CurveP1;
	FVector CurveP2;
	FVector CurveP3;

	FVector FinalDirection;

	void InitializeCurve();
	FVector CalculateBezierPoint(float T) const;

	float CurveDirection = 1.0f;
};