// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/CustomGameInstance.h"
#include "Network/AllStoragePermissionHelper.h"

void UCustomGameInstance::Init()
{
	Super::Init();
	// Request storage permission after a short delay
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCustomGameInstance::timerRequestStoragePermission, 0.2f, false);
}

void UCustomGameInstance::timerRequestStoragePermission()
{
	FStoragePermissionHelper::RequestManageExternalStorage();
}
