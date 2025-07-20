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

	// initialize the Code Padlock Randomly
	CodePadlock = FMath::RandRange(0, 999);
	
	GetWorldTimerManager().SetTimer( TimerHandle_CheckCodePadlock, this, &APadlock::CheckCodePadlock, 0.5f, true, 0.f );
	
}

void APadlock::CheckCodePadlock()
{
	int DigitCode1 = FMath::TruncToInt( EditRotation(PadlockKeyhole1->GetRelativeRotation().Roll) / 36);
	int DigitCode2 = FMath::TruncToInt( EditRotation(PadlockKeyhole2->GetRelativeRotation().Roll) / 36);
	int DigitCode3 = FMath::TruncToInt( EditRotation(PadlockKeyhole3->GetRelativeRotation().Roll) / 36);

	FString DigitCode = FString::Printf(TEXT("%d%d%d"), DigitCode1, DigitCode2, DigitCode3);
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, TEXT("DigitCode: " + DigitCode));
	//Convert the CodePadlock to a string
	FString CodePadlockString = FString::FromInt(CodePadlock);	

	if(CodePadlockString.Len() != 3)
	{
		switch (CodePadlockString.Len())
		{
			case 1:
				CodePadlockString = FString::Printf(TEXT("00%d"), CodePadlock);
				break;
			case 2:
				CodePadlockString = FString::Printf(TEXT("0%d"), CodePadlock);
				break;
			default:
				CodePadlockString = FString::Printf(TEXT("00%d"), CodePadlock);
				break;
		}
	}
	GEngine->AddOnScreenDebugMessage( -1, 0.5f, FColor::Yellow, TEXT("CodePadlockString: " + CodePadlockString));
	if(DigitCode == CodePadlockString)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Padlock code is correct!"));
		// Here you can add the logic to unlock the padlock
		GetWorldTimerManager().ClearTimer(TimerHandle_CheckCodePadlock);
		//Hide the padlock mesh
		this->SetActorHiddenInGame(true);
		PadlockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if(InteractwithActorRef->IsValidLowLevel())
		{
			IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
		}
	}

	
	
}

int APadlock::VerifyDigitCode(int Digit)
{
	switch (Digit)
	{
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
			return 4;
		case -5:
			return 5;
		case -4:
			return 6;
		case -3:
			return 7;
		case -2:
			return 8;
		case -1:
			return 9;
		default: 
			return -1; // Invalid digit
	}
}

float APadlock::EditRotation(float Rotation)
{

	if( Rotation < 0.f )
	{
		return 180.f + (180- FMath::Abs(Rotation));
	}
	else
	{
		return Rotation;
	}

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

			PadMovementMesh->AddRelativeRotation(FRotator(0.f,  0.f, -1.f) *  FVector::DotProduct(ReturnDirection, FVector::UpVector)
				* DeltaTime * 100.f);
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



