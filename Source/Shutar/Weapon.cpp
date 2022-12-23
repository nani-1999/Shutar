// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

AWeapon::AWeapon() : 
	AItem(),
	EnablePickUpTime{ 1.f }
{
	PrimaryActorTick.bCanEverTick = true;

	PickUpWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	WidgetTraceCollision->SetSphereRadius(150.f);
}

void AWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (ItemState == EItemState::EIS_Falling) {
		FRotator MeshUpRightRotation = FRotator(0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f);
		GetItemMesh()->SetWorldRotation(MeshUpRightRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

//void AWeapon::ThrowWeapon() {
//	FRotator MeshUpRightRotation = FRotator(0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f);
//	GetItemMesh()->SetWorldRotation(MeshUpRightRotation, false, nullptr, ETeleportType::TeleportPhysics);
//
//	FVector ForwardDirection = GetItemMesh()->GetRightVector();
//	FVector ImpulseDirection = ForwardDirection.RotateAngleAxis(FMath::FRandRange(-30.f, 30.f), FVector(0.f, 0.f, 1.f));
//	GetItemMesh()->AddImpulse(ImpulseDirection * 10'000.f);
//
//	GetWorld()->GetTimerManager().SetTimer(EnablePickUpTimerHandle, this, &AWeapon::EnablePickUp, EnablePickUpTime);
//}

void AWeapon::StartPickUpTimer() {
	GetWorld()->GetTimerManager().SetTimer(EnablePickUpTimerHandle, this, &AWeapon::EnablePickUp, EnablePickUpTime);
}
void AWeapon::EnablePickUp() {
	SetItemState(EItemState::EIS_PickUp);
}