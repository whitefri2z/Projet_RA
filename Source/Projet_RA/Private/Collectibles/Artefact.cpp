// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectibles/Artefact.h"

#include "GameFramework/GameStateBase.h"
#include "Player/CustomPlayerState.h"

// Sets default values
AArtefact::AArtefact()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArtefactMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArtefactMesh"));
	RootComponent = ArtefactMesh;
	ArtefactMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ArtefactMesh->SetGenerateOverlapEvents(true);
	ArtefactMesh->SetSimulatePhysics(false);
	ArtefactMesh->SetMobility(EComponentMobility::Static);
	ArtefactMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ArtefactMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

	ArtefactMesh->OnInputTouchBegin.AddDynamic(this, &AArtefact::OnInputTouchBegin);

}

// Called when the game starts or when spawned
void AArtefact::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArtefact::OnInputTouchBegin(const ETouchIndex::Type FingerIndex , UPrimitiveComponent* PrimitiveComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Artefact touched!"));

	Cast<ACustomPlayerState>( GetWorld()->GetGameState()->PlayerArray[0])->AddArtefact();
	
	Destroy();
}

// Called every frame
void AArtefact::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

