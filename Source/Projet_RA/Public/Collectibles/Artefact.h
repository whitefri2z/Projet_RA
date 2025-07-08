// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Artefact.generated.h"

UCLASS()
class PROJET_RA_API AArtefact : public AActor
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artefact", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ArtefactMesh;
	
protected:

public:

	// Sets default values for this actor's properties
	AArtefact();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Artefact")
	void OnInputTouchBegin(const ETouchIndex::Type FingerIndex , UPrimitiveComponent* PrimitiveComponent);



};
