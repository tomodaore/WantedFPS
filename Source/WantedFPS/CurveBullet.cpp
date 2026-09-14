#include "CurveBullet.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ACurveBullet::ACurveBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 当たり判定
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->InitSphereRadius(5.0f);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// 弾の見た目
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

	// Actorが向いている方向を弾の進行方向にする
	Direction = GetActorForwardVector();

	if (GetOwner()) {
		Collision->IgnoreActorWhenMoving(GetOwner(), true);
	}

	// 5秒経ったら自動削除
	SetLifeSpan(5.0f);
}

void ACurveBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 通常射撃
	if (CurveStrength <= KINDA_SMALL_NUMBER)
	{
		const FVector MoveAmount =
			Direction * Speed * DeltaTime;

		FHitResult Hit;

		AddActorWorldOffset(
			MoveAmount,
			true,
			&Hit
		);

		if (Hit.bBlockingHit)
		{
			Destroy();
		}

		return;
	}

	// カーブ弾の初期設定
	if (!bCurveInitialized)
	{
		InitializeCurve();
	}

	// ベジェ曲線上を移動
	if (!bCurveFinished)
	{
		const float SafeRange =
			FMath::Max(CurveRange, 1.0f);

		CurveT +=
			(Speed / SafeRange) * DeltaTime;

		CurveT =
			FMath::Clamp(CurveT, 0.0f, 1.0f);

		const FVector NextLocation =
			CalculateBezierPoint(CurveT);

		const FVector MoveAmount =
			NextLocation - GetActorLocation();

		FHitResult Hit;

		AddActorWorldOffset(
			MoveAmount,
			true,
			&Hit
		);

		if (Hit.bBlockingHit)
		{
			Destroy();
			return;
		}

		if (CurveT >= 1.0f)
		{
			bCurveFinished = true;
		}

		return;
	}

	// カーブ終了後は、その方向へ直進
	const FVector MoveAmount =
		FinalDirection * Speed * DeltaTime;

	FHitResult Hit;

	AddActorWorldOffset(
		MoveAmount,
		true,
		&Hit
	);

	if (Hit.bBlockingHit)
	{
		Destroy();
	}
}

void ACurveBullet::SetCurveStrength(float InCurveStrength)
{
	CurveStrength = FMath::Clamp(InCurveStrength, 0.0f, 1.0f);
}

void ACurveBullet::SetCurveDirection(float InCurveDirection)
{
	CurveDirection = InCurveDirection >= 0.0f ? 1.0f : -1.0f;
}

void ACurveBullet::InitializeCurve()
{
	bCurveInitialized = true;

	CurveP0 = GetActorLocation();

	const FVector Forward = GetActorForwardVector().GetSafeNormal();
	const FVector Right = GetActorRightVector().GetSafeNormal();

	// 最大チャージなら45度
	const float Angle = MaxCurveAngle * CurveStrength;

	/*
	 CurveDirection
	 -1 = 左カーブ
	 +1 = 右カーブ

	 左カーブの場合：
	 最初は右前方へ撃つ
	 最後は左前方へ向かう
	*/
	const float StartAngle =
		-CurveDirection * Angle;

	const float TargetAngle =
		CurveDirection * Angle;

	const float StartRad =
		FMath::DegreesToRadians(StartAngle);

	const float TargetRad =
		FMath::DegreesToRadians(TargetAngle);

	const FVector StartDirection =
		(
			Forward * FMath::Cos(StartRad) +
			Right * FMath::Sin(StartRad)
			).GetSafeNormal();

	const FVector TargetDirection =
		(
			Forward * FMath::Cos(TargetRad) +
			Right * FMath::Sin(TargetRad)
			).GetSafeNormal();

	// 最終到達点
	CurveP3 =
		CurveP0 +
		TargetDirection * CurveRange;

	const float HandleLength =
		CurveRange * CurveHandleRatio;

	// 発射直後の方向を決める
	CurveP1 =
		CurveP0 +
		StartDirection * HandleLength;

	// 終端へ入っていく方向を決める
	CurveP2 =
		CurveP3 -
		TargetDirection * HandleLength;

	FinalDirection = TargetDirection;
}

FVector ACurveBullet::CalculateBezierPoint(float T) const
{
	const float OneMinusT = 1.0f - T;

	return
		OneMinusT * OneMinusT * OneMinusT * CurveP0
		+
		3.0f * OneMinusT * OneMinusT * T * CurveP1
		+
		3.0f * OneMinusT * T * T * CurveP2
		+
		T * T * T * CurveP3;
}