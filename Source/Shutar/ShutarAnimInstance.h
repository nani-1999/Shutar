// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShutarAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHUTAR_API UShutarAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

	//properties
	class AShutarCharacter* ShutarCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	float Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	float Acceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	bool JogStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	bool JogStop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	bool JumpStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	bool JumpEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	float Angle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	float AngleOnStop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	float LookUpAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarAnimInstance | Properties")
	bool bAiming;

};
