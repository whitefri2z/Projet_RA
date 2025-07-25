// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInteract/Padlock.h"

#include "Components/TextRenderComponent.h"
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
	PadlockKeyhole4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole4"));
	PadlockKeyhole4->SetupAttachment(RootComponent);
	PadlockKeyhole4->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );
	PadlockKeyhole5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole5"));
	PadlockKeyhole5->SetupAttachment(RootComponent);
	PadlockKeyhole5->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );
	PadlockKeyhole6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole6"));
	PadlockKeyhole6->SetupAttachment(RootComponent);
	PadlockKeyhole6->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );
	PadlockKeyhole7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole7"));
	PadlockKeyhole7->SetupAttachment(RootComponent);
	PadlockKeyhole7->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );
	PadlockKeyhole8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole8"));
	PadlockKeyhole8->SetupAttachment(RootComponent);
	PadlockKeyhole8->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );
	PadlockKeyhole9 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole9"));
	PadlockKeyhole9->SetupAttachment(RootComponent);
	PadlockKeyhole9->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );
	PadlockKeyhole0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadlockKeyhole0"));
	PadlockKeyhole0->SetupAttachment(RootComponent);
	PadlockKeyhole0->OnInputTouchBegin.AddDynamic( this, &APadlock::OnInputTouchBeginCPP );

	PadlockText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PadlockText"));
	PadlockText->SetupAttachment(RootComponent);
	PadlockText->SetText(FText::FromString("___"));
	PadlockText->SetHorizontalAlignment(EHTA_Center);
	PadlockText->SetWorldSize(50.f);
	

	

}

void APadlock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void APadlock::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = GetWorld()->GetFirstPlayerController();

	// initialize the Code Padlock Randomly
	CodePadlock = FMath::RandRange(0, 999);
	GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Green, TEXT("Padlock code is: " + FString::FromInt(CodePadlock)));
	CodePadlockString = "___";
	
}


FString APadlock::ReplaceCharAt(const FString& OriginalString, int32 Index, TCHAR NewChar)
{
	if (!OriginalString.IsValidIndex(Index))
	{
		return OriginalString; // Index out of bounds, return original string
	}

	TArray<TCHAR> CharArray = OriginalString.GetCharArray();
	CharArray[Index] = NewChar;
	return FString(CharArray.GetData()).Left(CharArray.Num() - 1); // Convert back to FString and remove the null terminator
}

void APadlock::HideActor()
{
	SetActorHiddenInGame( true );
	SetActorEnableCollision( false );
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
				* DeltaTime * 50.f);
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
		
		
		if(PrimitiveComponent == PadlockKeyhole1)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '1');
		}
		if(PrimitiveComponent == PadlockKeyhole2)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '2');
		}
		if(PrimitiveComponent == PadlockKeyhole3)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '3');
		}
		if(PrimitiveComponent == PadlockKeyhole4)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '4');
		}
		if(PrimitiveComponent == PadlockKeyhole5)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '5');
		}
		if(PrimitiveComponent == PadlockKeyhole6)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '6');
		}
		if(PrimitiveComponent == PadlockKeyhole7)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '7');
		}
		if(PrimitiveComponent == PadlockKeyhole8)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '8');
		}
		if(PrimitiveComponent == PadlockKeyhole9)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '9');
		}
		if(PrimitiveComponent == PadlockKeyhole0)
		{
			CodePadlockString = ReplaceCharAt(CodePadlockString, CurrentDigit, '0');
		}
		CurrentDigit++;
		if(CurrentDigit >= NumbreOfDigits)
		{
			CurrentDigit = 0;
			if(CodePadlockString == FString::FromInt(CodePadlock))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Padlock code is correct!"));
				if(InteractwithActorRef->IsValidLowLevel())
				{
					IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
				}
				HideActor();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Padlock code is incorrect!"));
				CodePadlockString = "___";
			}
		}
		
		PadlockText->SetText(FText::FromString(CodePadlockString));
	}
}



