// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

 void AEnemyBase::HideEnemy()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

 void AEnemyBase::ShowEnemy()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

