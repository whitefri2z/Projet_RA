// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHitProjectile);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->InitialSpeed = 1000.0f; // Set initial speed
	ProjectileMovementComponent->MaxSpeed = 1000.0f; // Set max speed
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // Disable gravity
	ProjectileMovementComponent->bShouldBounce = false; // Disable bouncing
	ProjectileMovementComponent->bIsHomingProjectile = false; // Disable homing
	ProjectileMovementComponent->HomingAccelerationMagnitude = 0.0f; // No homing acceleration
	ProjectileMovementComponent->bForceSubStepping = false; // Disable sub-stepping
	ProjectileMovementComponent->bInterpMovement = false; // Disable interpolation
	ProjectileMovementComponent->bIsHomingProjectile = false; // Ensure homing is disabled


}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyProjectile::OnHitProjectile(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Actor hit : ") + OtherActor->GetName());
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

