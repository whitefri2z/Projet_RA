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
	//ProjectileMesh->SetCollisionProfileName(TEXT("OverlapAll")); // Set collision profile to overlap all
	ProjectileMesh->SetCollisionEnabled( ECollisionEnabled::Type::QueryAndPhysics ); // Enable collision for both query and physics
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHitProjectile);
	ProjectileMesh->OnInputTouchBegin.AddDynamic(this, &AEnemyProjectile::OnTouchBeginProjectile);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnBeginOverlapProjectile);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->InitialSpeed = 1000.0f; // Set initial speed
	ProjectileMovementComponent->MaxSpeed = 1000.0f; // Set max speed
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // Disable gravity
	ProjectileMovementComponent->Deactivate();

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
	SetActorEnableCollision( false ); // Disable collision after hit
	ProjectileMovementComponent->StopMovementImmediately(); // Stop the projectile movement
	ProjectileMesh->SetVisibility(false); // Hide the projectile mesh
	bIsUsed = false; // Mark the projectile as not used
	// If Other Actor Has IInteractionInterface, call the interface method
	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		IInteractionInterface::Execute_ActorHitByProjectile(OtherActor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("No interaction interface implemented on the hit actor."));
	}
	
}

void AEnemyProjectile::OnTouchBeginProjectile(ETouchIndex::Type ButtonPressed ,  UPrimitiveComponent* TouchedComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Projectile touched!"));
	// Implement any additional logic for touch input here
	ProjectileMovementComponent->Velocity = ProjectileMovementComponent->Velocity * -1; // Reverse the velocity on touch
}

void AEnemyProjectile::OnBeginOverlapProjectile(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("Projectile overlapped with: ") + OtherActor->GetName());
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

