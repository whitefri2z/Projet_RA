// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "Artefact.generated.h"

UENUM(BlueprintType)
enum class EArtefactDifficult : uint8
{
	Easy UMETA(DisplayName = "Easy"),
	Medium UMETA(DisplayName = "Medium"),
	Hard UMETA(DisplayName = "Hard"),
};

UENUM()
enum class EArtefactType : uint8
{
	Primary UMETA(DisplayName = "Primary"),
	Secondary UMETA(DisplayName = "Secondary"),
};

UCLASS()
class PROJET_RA_API AArtefact : public AActor , public IInteractionInterface
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artefact", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ArtefactMesh;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artefact", meta = (AllowPrivateAccess = "true"))
	EArtefactDifficult ArtefactDifficult = EArtefactDifficult::Easy;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Artefact", meta = (AllowPrivateAccess = "true"))
	EArtefactType ArtefactType = EArtefactType::Primary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artefact", meta = (AllowPrivateAccess = "true"))
	FString ArtefactName = "Unknown Artefact";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artefact", meta = (AllowPrivateAccess = "true"))
	FString ArtefactDescription = "No description available.";

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Artefact", meta = (AllowPrivateAccess = "true"))
	int ArtefactOrder = 0; // Order of the artefact in the collection
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Artefact", meta = (AllowPrivateAccess = "true"))
	int ArtefactID = -1; // Unique ID for the artefact, can be set in BeginPlay based on order or other logic


public:

	// Sets default values for this actor's properties
	AArtefact();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SuccessPuzzel_Implementation() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Artefact")
	void OnInputTouchBegin(const ETouchIndex::Type FingerIndex , UPrimitiveComponent* PrimitiveComponent);

	//Getters
	public:
	UFUNCTION(BlueprintCallable, Category = "Artefact")
	int GetArtefactID() const { return ArtefactID; };

	UFUNCTION( BlueprintCallable, Category = "Artefact" )
	int GetArtefactOrder() const { return ArtefactOrder; };



};
