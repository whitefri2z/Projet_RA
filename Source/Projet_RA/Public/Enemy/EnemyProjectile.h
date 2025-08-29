// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/InteractionInterface.h"
#include "EnemyProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PROJET_RA_API AEnemyProjectile : public APawn , public IInteractionInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectile | Sound" )
	TObjectPtr<USoundBase> ProjectileSpawnSound;

protected:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Projectile Variables")
	bool bIsUsed = false;

	FTimerHandle UnusedHandle;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION( BlueprintCallable, Category = "Projectile")
	void OnHitProjectile(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION( BlueprintCallable, Category = "Projectile")
	void OnTouchBeginProjectile(ETouchIndex::Type ButtonPressed ,  UPrimitiveComponent* TouchedComponent);

	UFUNCTION( BlueprintCallable, Category = "Projectile")
	void OnBeginOverlapProjectile(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HideProjectile();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	AEnemyProjectile();

	void SetIsUsed(bool bUsed) { bIsUsed = bUsed; }

	bool GetIsUsed() const { return bIsUsed; }

	void InitProjectile();

};
