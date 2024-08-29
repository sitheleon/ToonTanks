// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Tank가 존재할 경우에만 거리 계산
    if (Tank)
    {
        // Tower와 Tank 사이의 거리를 계산
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        // 사거리 내에 있는지 확인
        if (InFireRange())
        {
            // 사거리 내에 있으면 Turret을 회전시킴
            RotateTurret(Tank->GetActorLocation());
        }
    }
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();

}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    // Tank 객체를 가져옴
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    
    // 일정 시간 간격으로 CheckFireCondition 함수를 호출하도록 타이머 설정
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{

    if(Tank==nullptr)
    {
        return;
    }

    // Tank가 사거리 내에 있으면 발사
    if (InFireRange() && Tank->bAlive)
    {
        Fire();
    }
}

bool ATower::InFireRange()
{
    if (Tank)
    {
        // Tower와 Tank 사이의 거리를 계산하여 사거리 내에 있는지 확인
   float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if (Distance <= FireRange)
        {
            return true;
        }

    }

    return false;
}
