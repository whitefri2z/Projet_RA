// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/InteractionInterface.h"
#include "EnemyBase.generated.h"

class AEnemyProjectile;
class AGameStateAR;

UENUM( BlueprintType)
enum class EEnemyType : uint8
{
	Carcolh UMETA(DisplayName = "Carcolh"),
	Drak UMETA(DisplayName = "Drak"),
	Loup_Drape UMETA(DisplayName = "Loup Drape"),
	Tarasque UMETA(DisplayName = "Tarasque"),
};

UCLASS()
class PROJET_RA_API AEnemyBase : public APawn , public IInteractionInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> EnemySkeletal;


	

protected:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Enemy Info")
	EEnemyType EnemyType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Enemy Info")
	bool bIsActive = false;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Ref")
	TObjectPtr<AGameStateAR> GameStateARRef;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Drak")
	bool bIsVisibleDrak = false;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Drak")
	float DrakDistance = 500.0f;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Drak")
	int DraktakeDamage = 2;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Drak")
	bool bIsTakingDamage = true;

	UPROPERTY( EditAnywhere , BlueprintReadWrite, Category = "Enemy Loup Drape")
	TArray<TObjectPtr<AEnemyProjectile>> EnemyProjectile;

	UPROPERTY( VisibleAnywhere , BlueprintReadWrite, Category = "Enemy Loup Drape")
	int LoupDrapeHitCount = 0;

	FTimerHandle EnemySpawnTimerHandle;
	FTimerDelegate EnemySpawnDelegate;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION( BlueprintCallable, Category = "Enemy - Carcolh")
	void ActionCarcolh();

	UFUNCTION( BlueprintCallable, Category = "Enemy - Drak")
	void ActionDrak();

	UFUNCTION( BlueprintCallable, Category = "Enemy - Loup Drape")
	void ActionLoupDrape();

	UFUNCTION( BlueprintCallable, Category = "Enemy - Loup Drape")
	void ThrowProjectile();

	UFUNCTION( BlueprintCallable, Category = "Enemy - Tarasque")
	void ActionTarasque();

	UFUNCTION(Blueprintable, BlueprintNativeEvent , Category = "EnemyMesh")
	void UpdateEnemyMesh();

	UFUNCTION( BlueprintCallable, Category = "OnTouch")
	void OnTouchEnemy(ETouchIndex::Type ButtonPressed, UPrimitiveComponent* TouchedComponent);

public:

	// Sets default values for this pawn's properties
	AEnemyBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void HideEnemy();

	UFUNCTION( BlueprintCallable, Category = "Enemy")
	void ShowEnemy();

	UFUNCTION( BlueprintCallable, Category = "Enemy")
	void InitEnemy(EEnemyType NewEnemyType , AGameStateAR* GameStateAR);

	//Implementation of the interaction interface
	virtual  void ActorHitByProjectile_Implementation() override;


};
