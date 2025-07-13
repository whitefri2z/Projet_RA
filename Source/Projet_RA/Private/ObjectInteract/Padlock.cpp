// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInteract/Padlock.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
APadlock::APadlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PadlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockMesh"));
	RootComponent = PadlockMesh;

	PadlockKeyhole1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole1"));
	PadlockKeyhole1->SetupAttachment(RootComponent);
	PadlockKeyhole1->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );
	PadlockKeyhole2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole2"));
	PadlockKeyhole2->SetupAttachment(RootComponent);
	PadlockKeyhole2->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );
	PadlockKeyhole3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole3"));
	PadlockKeyhole3->SetupAttachment(RootComponent);
	PadlockKeyhole3->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );

	

}

void APadlock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	/*
	for (UStaticMeshComponent* Keyhole : PadlockKeyhole)
	{
		if (Keyhole)
		{
			Keyhole->DestroyComponent();
		}
	}
	PadlockKeyhole.Empty();

	for (int i=0; i < NumbreOfDigits; i++)
	{
		PadlockKeyhole.Add(NewObject<UStaticMeshComponent>(this));
		PadlockKeyhole[i]->SetupAttachment(RootComponent);
		UE_LOG(LogTemp, Warning, TEXT("PadlockKeyhole[%d] created."), i);
	}*/
}

// Called when the game starts or when spawned
void APadlock::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = GetWorld()->GetFirstPlayerController();

	GetWorldTimerManager().SetTimer( TimerHandle_CheckCodePadlock, this, &APadlock::CheckCodePadlock, 0.5f, true, 0.f );
	
}

void APadlock::CheckCodePadlock()
{
	float DigitCode = FMath::Floor( PadlockKeyhole1->GetRelativeRotation().Roll / 36) ;
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("DigitCode : " + FString::SanitizeFloat(DigitCode)));

	
}

// Called every frame
void APadlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsMoving && PadMovementMesh->IsValidLowLevel() && PlayerControllerRef)
	{
		bool bIsPressed;
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Type::Touch1, NewPositionTouch.X, NewPositionTouch.Y, bIsPressed);
		if(bIsPressed)
		{
			FVector StartWorldPositionTouch;
			FVector StartWorldDirectionTouch;
			FVector NewWorldPositionTouch;
			FVector NewWorldDirectionTouch;
			
			PlayerControllerRef->DeprojectScreenPositionToWorld(StartPositionTouch.X, StartPositionTouch.Y,
				StartWorldPositionTouch, StartWorldDirectionTouch);
			PlayerControllerRef->DeprojectScreenPositionToWorld(NewPositionTouch.X, NewPositionTouch.Y,
				NewWorldPositionTouch, NewWorldDirectionTouch);

			FVector ReturnDirection = UKismetMathLibrary::GetDirectionUnitVector(StartWorldPositionTouch, NewWorldPositionTouch);

			PadMovementMesh->AddRelativeRotation(FRotator(0.f,  0.f, -1.f) *  FVector::DotProduct(ReturnDirection, FVector::RightVector)
				* DeltaTime * 1000.f);
		}
		else
		{
			bIsMoving = false;
			PadMovementMesh = nullptr;
		}
	}

}

void APadlock::OnInputTouchBeginCPP(const ETouchIndex::Type FingerIndex, UPrimitiveComponent* PrimitiveComponent)
{
	GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("PrimitiveComponent touched : "
																			+ PrimitiveComponent->GetName()));

	if(PrimitiveComponent->IsValidLowLevel() && FingerIndex == ETouchIndex::Type::Touch1 && PlayerControllerRef->IsValidLowLevel())
	{
		bool bIsPressed;
		PlayerControllerRef->GetInputTouchState(FingerIndex, StartPositionTouch.X, StartPositionTouch.Y, bIsPressed);
		PadMovementMesh = Cast<UStaticMeshComponent>(PrimitiveComponent);
		bIsMoving = true;
	}
}



