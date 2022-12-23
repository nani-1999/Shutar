// Fill out your copyright notice in the Description page of Project Settings.


#include "ShutarCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Weapon.h"
#include "Components/WidgetComponent.h"

// Sets default values
AShutarCharacter::AShutarCharacter() :
	ACharacter(),
	bAiming{ false },
	CameraHipFOV{ 0.f },
	CameraAimFOV{ 30.f },

	BaseLookUp{ 1.f },
	BaseTurn{ 1.f },
	HipLookUp{ 0.9f },
	HipTurn{ 0.9f },
	AimLookUp{ 0.7f },
	AimTurn{ 0.7f },

	bIsFiring{ false },
	bIsAllowedToFireBullet{ true },
	FireRate{ 0.2 },

	CrosshairSpreadRate{ 0.f },
	CrosshairHipSpreadRate{ 0.5f },
	CrosshairVelocitySpreadRate{ 0.f },
	CrosshairAimSpreadRate{ 0.f },
	CrosshairAirSpreadRate{ 0.f },
	CrosshairFireSpreadRate{ 0.f },

	ItemOverlappingCount{ 0 },

	ItemInterpDistance{ 150.f },
	ItemInterpElevation{ 15.f }
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 75.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
}

// Called when the game starts or when spawned
void AShutarCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (FollowCamera) {
		float CameraDefaultFOV = FollowCamera->FieldOfView;
		CameraHipFOV = CameraDefaultFOV;
	}

	SetBaseSensitivity(HipLookUp, HipTurn);

	EquipWeapon(SpawnDefaultWeapon());
}

// Called every frame
void AShutarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FollowCameraHipToAimInterp(DeltaTime);

	CalculateCrosshairSpreadRate(DeltaTime);

	ItemWidgetTrace();
}

// Called to bind functionality to input
void AShutarCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShutarCharacter::FirePressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &AShutarCharacter::FireReleased);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &AShutarCharacter::AimPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &AShutarCharacter::AimReleased);
	PlayerInputComponent->BindAction(TEXT("Select"), EInputEvent::IE_Pressed, this, &AShutarCharacter::SelectPressed);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShutarCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShutarCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShutarCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShutarCharacter::Turn);

	//base sensitivity
	//baserate sensitivity
}

void AShutarCharacter::MoveForward(float Value) {
	if (Value && Controller) {
		FRotator ControllerRotationYaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		FVector ForwardDirection = FRotationMatrix(ControllerRotationYaw).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, Value);
	}
}
void AShutarCharacter::MoveRight(float Value) {
	if (Value && Controller) {
		FRotator ControllerRotationYaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		FVector RightDirection = FRotationMatrix(ControllerRotationYaw).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, Value);
	}
}
void AShutarCharacter::LookUp(float Value) {
	if (Value && Controller) {
		AddControllerPitchInput(Value * BaseLookUp);
	}
}
void AShutarCharacter::Turn(float Value) {
	if (Value && Controller) {
		AddControllerYawInput(Value * BaseTurn);
	}
}

void AShutarCharacter::FirePressed() {
	bIsFiring = true;
	FireWeapon();
}
void AShutarCharacter::FireReleased() {
	bIsFiring = false;
}
void AShutarCharacter::FireWeapon() {
	if (bIsAllowedToFireBullet) {
		FireBullet();
		bIsAllowedToFireBullet = false;
		GetWorld()->GetTimerManager().SetTimer(AllowBulletFireTimerHandle, this, &AShutarCharacter::AllowBulletFire, FireRate);
	}
}
void AShutarCharacter::AllowBulletFire() {
	bIsAllowedToFireBullet = true;
	if (bIsFiring) FireWeapon();
}
void AShutarCharacter::FireBullet() {
	if (FireSound) UGameplayStatics::PlaySound2D(this, FireSound);

	const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName(TEXT("WeaponFireSocket"));
	if (WeaponSocket) {
		FTransform WeaponSocketTransform = WeaponSocket->GetSocketTransform(GetMesh());
		if (MuzzleFlash) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, WeaponSocketTransform);

		FVector FireImpactLocation;
		if (GetFireImpaceLocation(WeaponSocketTransform.GetLocation(), FireImpactLocation)) {
			if (ImpactParticle) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, FireImpactLocation);
			if (BeamParticle) {
				UParticleSystemComponent* SpawnedBeamParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticle, WeaponSocketTransform);
				if (SpawnedBeamParticle) {
					SpawnedBeamParticle->SetVectorParameter(TEXT("Target"), FireImpactLocation);
				}
			}
		}
	}

	UAnimInstance* ShutarAnimInstance = GetMesh()->GetAnimInstance();
	if (ShutarAnimInstance && ShutarAnimMontage) {
		ShutarAnimInstance->Montage_Play(ShutarAnimMontage);
		ShutarAnimInstance->Montage_JumpToSection("Fire");
	}

	//CrosshairFireSpread
	bCrosshairFireSpread = true;
	GetWorld()->GetTimerManager().SetTimer(CrosshairFireSpreadTimerHandle, this, &AShutarCharacter::CrosshairFireSpreadStop, 0.2f);
	//end of CrosshairFireSpread
}

void AShutarCharacter::AimPressed() {
	bAiming = true;
	SetBaseSensitivity(AimLookUp, AimTurn);
}
void AShutarCharacter::AimReleased() {
	bAiming = false;
	SetBaseSensitivity(HipLookUp, HipTurn);
}

float AShutarCharacter::GetAngle() const {
	FVector Forward = GetActorForwardVector();
	float ForwardAngle = FMath::Atan2(Forward.Y, Forward.X) * (180.f / PI);

	FVector Movement = GetVelocity();
	float MovementAngle = FMath::Atan2(Movement.Y, Movement.X) * (180.f / PI);

	float DifferenceAngle = (MovementAngle != 0.f) ? MovementAngle - ForwardAngle : 0.f;
	if (DifferenceAngle > 180.f) DifferenceAngle -= 360.f;
	if (DifferenceAngle < -180.f) DifferenceAngle += 360.f;

	return DifferenceAngle;
}

bool AShutarCharacter::GetCrosshairScreenLocation(FVector2D& Location) const {
	FVector2D CrosshairScreenLocation = FVector2D::ZeroVector;

	if (!(GEngine && GEngine->GameViewport)) {
		Location = CrosshairScreenLocation;
		return false;
	}

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D ViewportCenter = ViewportSize / 2.f;

	CrosshairScreenLocation = ViewportCenter;

	Location = CrosshairScreenLocation;
	return true;
}

bool AShutarCharacter::GetCrosshairTraceHit(float TraceLength, FHitResult& OutHit, FVector& EndLocation) {
	FVector2D CrosshairScreenLocation;
	if (!GetCrosshairScreenLocation(CrosshairScreenLocation)) return false;

	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairScreenLocation, CrosshairWorldLocation, CrosshairWorldDirection)) return false;

	FHitResult CrosshairTraceHit;
	const FVector CrosshairStart = CrosshairWorldLocation;
	const FVector CrosshairEnd = CrosshairWorldLocation + CrosshairWorldDirection * TraceLength;
	GetWorld()->LineTraceSingleByChannel(CrosshairTraceHit, CrosshairStart, CrosshairEnd, ECollisionChannel::ECC_Visibility);
	OutHit = CrosshairTraceHit;
	EndLocation = CrosshairEnd;
	return true;
}
bool AShutarCharacter::GetFireImpaceLocation(const FVector& SocketLocation, FVector& ImpactLocation) {
	FHitResult CrosshairTraceHit;
	FVector CrosshairEnd;
	if (!GetCrosshairTraceHit(50'000.f, CrosshairTraceHit, CrosshairEnd)) return false;

	if (CrosshairTraceHit.bBlockingHit) {
		FHitResult FireTraceHit;
		const FVector FireStart = SocketLocation;
		const FVector FireEnd = 2.f * CrosshairTraceHit.Location - SocketLocation;
		GetWorld()->LineTraceSingleByChannel(FireTraceHit, FireStart, FireEnd, ECollisionChannel::ECC_Visibility);
		if (FireTraceHit.bBlockingHit) {
			ImpactLocation = FireTraceHit.Location;
			DrawDebugPoint(GetWorld(), ImpactLocation, 5.f, FColor::Cyan, false, 5.f); //============DrawDebug============
			return true;
		}
	}
	ImpactLocation = CrosshairEnd;
	return true;
}

void AShutarCharacter::FollowCameraHipToAimInterp(float DeltaTime) {
	if (FollowCamera) {
		if (bAiming) {
			FollowCamera->SetFieldOfView(FMath::FInterpTo(FollowCamera->FieldOfView, CameraAimFOV, DeltaTime, 30.f));
		}
		else {
			FollowCamera->SetFieldOfView(FMath::FInterpTo(FollowCamera->FieldOfView, CameraHipFOV, DeltaTime, 30.f));
		}
	}
}

void AShutarCharacter::SetBaseSensitivity(float NewLookUp, float NewTurn) {
	BaseLookUp = NewLookUp;
	BaseTurn = NewTurn;
}

void AShutarCharacter::CalculateCrosshairSpreadRate(float DeltaTime) {
	//Aim
	if (bAiming) {
		CrosshairAimSpreadRate = FMath::FInterpTo(CrosshairAimSpreadRate, 0.5f, DeltaTime, 30.f);
	}
	else {
		CrosshairAimSpreadRate = FMath::FInterpTo(CrosshairAimSpreadRate, 0.f, DeltaTime, 30.f);
	}

	//Velocity
	FVector2D VelocityRange(0.f, GetCharacterMovement()->MaxWalkSpeed);
	FVector2D CrosshairVelocitySpreadRateRange(0.f, 1.f);
	float Velocity = GetVelocity().Size2D();
	CrosshairVelocitySpreadRate = FMath::GetMappedRangeValueClamped(VelocityRange, CrosshairVelocitySpreadRateRange, Velocity);

	//Air
	if (GetCharacterMovement()->IsFalling()) {
		CrosshairAirSpreadRate = FMath::FInterpTo(CrosshairAirSpreadRate, 2.25f, DeltaTime, 10.f);
	}
	else {
		CrosshairAirSpreadRate = FMath::FInterpTo(CrosshairAirSpreadRate, 0.f, DeltaTime, 10.f);
	}

	//Fire
	if (bCrosshairFireSpread) {
		CrosshairFireSpreadRate = FMath::FInterpTo(CrosshairFireSpreadRate, 0.5f, DeltaTime, 20.f);
	}
	else {
		CrosshairFireSpreadRate = FMath::FInterpTo(CrosshairFireSpreadRate, 0.f, DeltaTime, 20.f);
	}

	CrosshairSpreadRate = CrosshairHipSpreadRate - CrosshairAimSpreadRate + CrosshairVelocitySpreadRate + CrosshairAirSpreadRate + CrosshairFireSpreadRate;
}

void AShutarCharacter::ItemWidgetTrace() {
	AItem* CurrentTracedItem = nullptr;
	if (ItemOverlappingCount) {
		FHitResult HitItem;
		FVector HitEnd;
		GetCrosshairTraceHit(500.f, HitItem, HitEnd);
		if (HitItem.bBlockingHit) {
			CurrentTracedItem = Cast<AItem>(HitItem.GetActor());
			if (CurrentTracedItem) {
				CurrentTracedItem->GetPickUpWidgetComponent()->SetVisibility(true);
			}
		}
	}
	if (TracedItem && TracedItem != CurrentTracedItem) {
		TracedItem->GetPickUpWidgetComponent()->SetVisibility(false);
		TracedItem = nullptr;
	}
	else {
		TracedItem = CurrentTracedItem;
	}
}

void AShutarCharacter::IncrementItemOverlappingCount(int8 Amount) {
	if (ItemOverlappingCount + Amount >= 0) {
		ItemOverlappingCount += Amount;
	}
}

void AShutarCharacter::SelectPressed() {
	//AWeapon* TracedWeapon = Cast<AWeapon>(TracedItem);
	//SwapWeapon(TracedWeapon);

	if (TracedItem) TracedItem->ItemInterpStart(this);
}

AWeapon* AShutarCharacter::SpawnDefaultWeapon() {
	if (DefaultWeaponBP) return GetWorld()->SpawnActor<AWeapon>(DefaultWeaponBP);
	return nullptr;
}

void AShutarCharacter::EquipWeapon(AWeapon* WeaponToEquip) {
	if (WeaponToEquip) {
		//const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName(TEXT("WeaponSocket"));
		bool WeaponSocket = GetMesh()->DoesSocketExist(TEXT("WeaponSocket"));
		if (WeaponSocket) {
			WeaponToEquip->SetItemState(EItemState::EIS_Equipped);
			//WeaponSocket->AttachActor(WeaponToEquip, GetMesh());
			FAttachmentTransformRules WeaponAttachmentRule(FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			WeaponToEquip->GetItemMesh()->AttachToComponent(GetMesh(), WeaponAttachmentRule, TEXT("WeaponSocket"));
			EquippedWeapon = WeaponToEquip;
		}
	}
}
void AShutarCharacter::UnEquipWeapon() {
	if (EquippedWeapon) {
		FDetachmentTransformRules WeaponDetachmentRule(FDetachmentTransformRules::KeepWorldTransform);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(WeaponDetachmentRule);
		EquippedWeapon->SetItemState(EItemState::EIS_Falling);
		//EquippedWeapon->ThrowWeapon();
		FVector ImpulseDirection = GetActorForwardVector().RotateAngleAxis(FMath::FRandRange(0.f, 45.f), FVector(0.f, 0.f, 1.f));
		EquippedWeapon->GetItemMesh()->AddImpulse(ImpulseDirection * 3'000.f);
		EquippedWeapon->StartPickUpTimer();
		EquippedWeapon = nullptr;
	}
}

void AShutarCharacter::SwapWeapon(AWeapon* WeaponToSwap) {
	UnEquipWeapon();
	EquipWeapon(WeaponToSwap);
}

FVector AShutarCharacter::GetItemInterpTargetLocation() const {
	return FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * ItemInterpDistance + FollowCamera->GetUpVector() * ItemInterpElevation;
}
FRotator AShutarCharacter::GetIteminterpTargeTRotation() const {
	return FollowCamera->GetForwardVector().Rotation();
}

void AShutarCharacter::EquipItem(AItem* ItemToEquip) {
	auto Weapon = Cast<AWeapon>(ItemToEquip);
	if (Weapon) {
		SwapWeapon(Weapon);
	}
	//else if (Item) //consume;
}