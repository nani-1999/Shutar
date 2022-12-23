// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SHUTAR_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	FTimerHandle EnablePickUpTimerHandle;
	float EnablePickUpTime;
	void EnablePickUp();

public:
	//void ThrowWeapon();
	void StartPickUpTimer();
};
