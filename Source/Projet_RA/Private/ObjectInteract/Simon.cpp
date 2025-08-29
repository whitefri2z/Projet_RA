// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInteract/Simon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASimon::ASimon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SimonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SimonMesh"));
	RootComponent = SimonMesh;
	SimonMesh->OnInputTouchBegin.AddDynamic( this, &ASimon::OnTouchBeginCPP );

	SimonButtonMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SimonButtonMesh1"));
	SimonButtonMesh1->SetupAttachment(RootComponent);
	SimonButtonMesh1->OnInputTouchBegin.AddDynamic( this, &ASimon::OnTouchBeginCPP );
	SimonButtonMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SimonButtonMesh2"));
	SimonButtonMesh2->SetupAttachment(RootComponent);
	SimonButtonMesh2->OnInputTouchBegin.AddDynamic( this, &ASimon::OnTouchBeginCPP );
	SimonButtonMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SimonButtonMesh3"));
	SimonButtonMesh3->SetupAttachment(RootComponent);
	SimonButtonMesh3->OnInputTouchBegin.AddDynamic( this, &ASimon::OnTouchBeginCPP );
	SimonButtonMesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SimonButtonMesh4"));
	SimonButtonMesh4->SetupAttachment(RootComponent);
	SimonButtonMesh4->OnInputTouchBegin.AddDynamic( this, &ASimon::OnTouchBeginCPP );

	ScenePlayerTP = CreateDefaultSubobject<USceneComponent>(TEXT("ScenePlayerTP"));
	ScenePlayerTP->SetupAttachment(RootComponent);
	

}

// Called when the game starts or when spawned
void ASimon::BeginPlay()
{
	Super::BeginPlay();
	PlayerControllerRef = GetWorld()->GetFirstPlayerController();
	GenerateSimonSequence();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Simon puzzle started!"));
	
}

void ASimon::GenerateSimonSequence()
{
	for(int i=0; i < MaxSteps; i++)
	{
		int RandomButton = FMath::RandRange(1, 4); // Assuming buttons are numbered 1 to 4
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Yellow, FString::Printf(TEXT("Random Button: %d"), RandomButton));
		SimonSequence.Add(RandomButton);
	}
	CurrentStep = 0; // Reset the current step
	SequenceStep = 0; // Reset the sequence step

	PlaySimonSequence();

}

void ASimon::ColorButton_Implementation(int StepIndex, FLinearColor ButtonColor)
{
}


void ASimon::ColorAllButtons_Implementation(FLinearColor ButtonColor)
{
}

void ASimon::CheckPlayerInput(int PlayerInput)
{
	if(SimonSequence.Num() == 0 || CurrentStep >= MaxSteps)
	{
		GenerateSimonSequence();
		return;
	}
	GetWorldTimerManager().ClearTimer( SequenceTimerHandle ); // Clear any existing timer
	GetWorldTimerManager().SetTimer( SequenceTimerHandle, this, &ASimon::PlaySimonSequence, 5.0f, false ); // Reset the sequence timer

	if(PlayerInput == SimonSequence[CurrentStep])
	{
		ColorButton( PlayerInput-1, FLinearColor::Green ); // Change button color to green on correct input
		CurrentStep++;
		UGameplayStatics::PlaySound2D(GetWorld(), CorrectButtonSound);
		if(CurrentStep >= MaxSteps)
		{
			// Player completed the sequence
			ColorAllButtons( FLinearColor::Green ); // Change all buttons to green on completion
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Congratulations! You completed the Simon sequence!"));
			if(SimonArtefact->IsValidLowLevel())
			{
				IInteractionInterface::Execute_SuccessPuzzel( SimonArtefact );
			}
			GetWorldTimerManager().ClearTimer( SequenceTimerHandle ); // Clear the timer after completion
			GetWorldTimerManager().SetTimer( SequenceTimerHandle, this, &ASimon::HideActor, 2.0f, false ); // Hide the actor after a short delay
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Wrong input! Try again."));
		CurrentStep = 0; // Reset the current step on wrong input
		ColorAllButtons(FLinearColor::Red); // Change all buttons to red on wrong input
		GetWorldTimerManager().SetTimer( SequenceTimerHandle, this, &ASimon::PlaySimonSequence, 1.f, false ); // Restart the sequence after a short delay
		UGameplayStatics::PlaySound2D(GetWorld(), WrongButtonSound);
	}
}

void ASimon::PlaySimonSequence()
{
	if(SimonSequence.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("No sequence to play."));
		return;
	}
	if (SimonSequence.IsValidIndex(SequenceStep))
	{
		ColorButton(SimonSequence[SequenceStep]-1.f, FLinearColor::Blue ); // Reset button colors before playing sequence
	}
	
	SequenceStep++;

	if(SequenceStep <= SimonSequence.Num())
	{
		GetWorldTimerManager().SetTimer( SequenceTimerHandle, this, &ASimon::PlaySimonSequence, 1.f, false );
	}
	else
	{
		// Sequence completed, reset step
		SequenceStep = 0;
		GetWorldTimerManager().SetTimer( SequenceTimerHandle, this, &ASimon::PlaySimonSequence, 4.0f, false );
	}
}

void ASimon::HideActor()
{
	this->SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);
}

void ASimon::ShowActor()
{ 
	this->SetActorHiddenInGame(false);
	this->SetActorEnableCollision(true);
	GenerateSimonSequence();
}

void ASimon::OnTouchBeginCPP(const ETouchIndex::Type FingerIndex, UPrimitiveComponent* PrimitiveComponent)
{
	if((GetActorLocation() - PlayerControllerRef->GetPawn()->GetActorLocation()).Size() > 1000.f)
	{
		IInteractionInterface::Execute_FocusOnActor(PlayerControllerRef->GetPawn(), ScenePlayerTP->GetComponentLocation());
	}
	GEngine->AddOnScreenDebugMessage( -1,5.f, FColor::Emerald, TEXT("Component Touch : " + PrimitiveComponent->GetName()));
	if (PrimitiveComponent == SimonButtonMesh1)
	{
		CheckPlayerInput(1);
	}
	else if (PrimitiveComponent == SimonButtonMesh2)
	{
		CheckPlayerInput(2);
	}
	else if (PrimitiveComponent == SimonButtonMesh3)
	{
		CheckPlayerInput(3);
	}
	else if (PrimitiveComponent == SimonButtonMesh4)
	{
		CheckPlayerInput(4);
	}
}

// Called every frame
void ASimon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


