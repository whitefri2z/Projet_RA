// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Padlock.generated.h"

UCLASS()
class PROJET_RA_API APadlock : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "PadlockMesh")
	TObjectPtr<UStaticMeshComponent> PadlockMesh;

	/*UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "PadlockMesh" )
	TArray<TObjectPtr<UStaticMeshComponent>> PadlockKeyhole;*/

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "PadlockMesh" )
	TObjectPtr<UStaticMeshComponent> PadlockKeyhole1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "PadlockMesh" )
	TObjectPtr<UStaticMeshComponent> PadlockKeyhole2;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "PadlockMesh" )
	TObjectPtr<UStaticMeshComponent> PadlockKeyhole3;
	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PadlockVariables")
	TObjectPtr<APlayerController> PlayerControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PadlockVariables")
	TObjectPtr<UStaticMeshComponent> PadMovementMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PadlockVariables")
	FVector2D StartPositionTouch;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "PadlockVariables" )
	FVector2D NewPositionTouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PadlockVariables")
	bool bIsMoving;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "CodePadlock")
	int NumbreOfDigits;
	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "CodePadlock")
	int CodePadlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimerPadlock")
	FTimerHandle TimerHandle_CheckCodePadlock;


	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	UFUNCTION(Blueprintable, Category = "Padlock")
	void CheckCodePadlock();

public:

	// Sets default values for this actor's properties
	APadlock();	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION( BlueprintCallable, Category = "Padlock" )
	void OnInputTouchBeginCPP(const ETouchIndex::Type FingerIndex, UPrimitiveComponent* PrimitiveComponent);

};
