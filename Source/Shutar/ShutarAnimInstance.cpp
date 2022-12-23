// Fill out your copyright notice in the Description page of Project Settings.


#include "ShutarAnimInstance.h"
#include "ShutarCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UShutarAnimInstance::NativeInitializeAnimation() {
	if (ShutarCharacter == nullptr) {
		ShutarCharacter = Cast<AShutarCharacter>(TryGetPawnOwner());
	}
}

void UShutarAnimInstance::UpdateAnimationProperties() {
	if (ShutarCharacter == nullptr) {
		ShutarCharacter = Cast<AShutarCharacter>(TryGetPawnOwner());
	}

	if (ShutarCharacter) {
		Velocity = ShutarCharacter->GetVelocity().Size2D();
		Acceleration = ShutarCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size2D();
		bIsInAir = ShutarCharacter->GetCharacterMovement()->IsFalling();
		JogStart = (Acceleration && Velocity && !bIsInAir);
		JogStop = !JogStart;
		JumpStart = bIsInAir;
		JumpEnd = !bIsInAir;

		Angle = ShutarCharacter->GetAngle();
		AngleOnStop = (Angle != 0.f) ? Angle : AngleOnStop;

		if (ShutarCharacter->Controller) {
			float PitchAngle = ShutarCharacter->Controller->GetControlRotation().Pitch;
			LookUpAngle = (PitchAngle >= 270.f) ? PitchAngle - 360.f : PitchAngle;
			LookUpAngle += 10.f;
		}

		bAiming = ShutarCharacter->IsAiming();
	}
}