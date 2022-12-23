// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShutarCharacter.generated.h"

UCLASS()
class SHUTAR_API AShutarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShutarCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//components
	UPROPERTY(VisibleAnywhere, Category = "ShutarCharacter | Components")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "ShutarCharacter | Components")
	class UCameraComponent* FollowCamera;

	//key binds
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void FirePressed();
	void FireReleased();
	void AimPressed();
	void AimReleased();
	void SelectPressed();

	//Interpolation
	void FollowCameraHipToAimInterp(float DeltaTime);

	//Animation
	UPROPERTY(EditAnywhere, Category = "ShutarCharacter | Animation", meta = (AllowPrivateAccess))
	class UAnimMontage* ShutarAnimMontage;

	//particles
	UPROPERTY(EditAnywhere, Category = "ShutarCharacter | Particles", meta = (AllowPrivateAccess))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Shutarcharacter | Particles", meta = (AllowPrivateAccess))
	class UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "Shutarcharacter | Particles", meta = (AllowPrivateAccess))
	class UParticleSystem* BeamParticle;

	//sound
	UPROPERTY(EditAnywhere, Category = "ShutarCharacter | Sound", meta = (AllowPrivateAccess))
	class USoundCue* FireSound;

	//aim zoom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShutarCharacter | Aim", meta = (AllowPrivateAccess))
	bool bAiming;
	float CameraHipFOV;
	float CameraAimFOV;

	//sensitivity
	float BaseLookUp;
	float BaseTurn;
	void SetBaseSensitivity(float NewLookUp, float NewTurn);
	float HipLookUp;
	float HipTurn;
	float AimLookUp;
	float AimTurn;

	//fire
	UPROPERTY(VisibleAnywhere, Category = "ShutarCharacter | Fire", meta = (AllowPrivateAccess))
	bool bIsFiring;
	void FireWeapon();

	UPROPERTY(VisibleAnywhere, Category = "ShutarCharacter | Fire", meta = (AllowPrivateAccess))
	bool bIsAllowedToFireBullet;
	void FireBullet();
	FTimerHandle AllowBulletFireTimerHandle;
	void AllowBulletFire();

	UPROPERTY(EditAnywhere, Category = "ShutarCharacter | Fire", meta = (AllowPrivateAccess))
	float FireRate;

	//crosshair
	float CrosshairSpreadRate;
	void CalculateCrosshairSpreadRate(float DeltaTime);

	float CrosshairHipSpreadRate;
	float CrosshairVelocitySpreadRate;
	float CrosshairAimSpreadRate;
	float CrosshairAirSpreadRate;
	float CrosshairFireSpreadRate;

	bool bCrosshairFireSpread;
	FTimerHandle CrosshairFireSpreadTimerHandle;
	void CrosshairFireSpreadStop() { bCrosshairFireSpread = false; }

	//trace
	bool GetCrosshairTraceHit(float TraceLength, FHitResult& OutHit, FVector& EndLocation);
	bool GetFireImpaceLocation(const FVector& SocketLocation, FVector& ImpactLocation);

	void ItemWidgetTrace();
	UPROPERTY(VisibleAnywhere, Category = "ShutarCharacter | Trace", meta = (AllowPrivateAccess))
	int8 ItemOverlappingCount;
	UPROPERTY(VisibleAnywhere, Category = "ShutarCharacter | Trace", meta = (AllowPrivateAccess))
	class AItem* TracedItem;

	//weapon
	UPROPERTY(VisibleAnywhere, Category = "ShutarCharacter | Weapon", meta = (AllowPrivateAccess))
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = "ShutarCharacter | Weapon", meta = (AllowPrivateAccess))
	TSubclassOf<AWeapon> DefaultWeaponBP;
	AWeapon* SpawnDefaultWeapon();

	void EquipWeapon(AWeapon* WeaponToEquip);
	void UnEquipWeapon();
	void SwapWeapon(AWeapon* WeaponToSwap);

	//Item
	UPROPERTY(EditAnywhere, Category = "ShutarCharacter | Item", meta = (AllowPrivateAccess))
	float ItemInterpDistance;
	UPROPERTY(EditAnywhere, Category = "ShutarCharacter | Item", meta = (AllowPrivateAccess))
	float ItemInterpElevation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//getters
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool IsAiming() const { return bAiming; }
	FORCEINLINE float GetCrosshairSpreadRate() const { return CrosshairSpreadRate; }

	float GetAngle() const;

	bool GetCrosshairScreenLocation(FVector2D& Location) const;

	void IncrementItemOverlappingCount(int8 Amount);

	FVector GetItemInterpTargetLocation() const;
	FRotator GetIteminterpTargeTRotation() const;
	void EquipItem(class AItem* ItemToEquip);
};
