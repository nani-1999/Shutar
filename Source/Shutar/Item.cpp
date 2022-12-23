// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "PickUpWidget.h"
#include "Components/SphereComponent.h"
#include "ShutarCharacter.h"

// Sets default values
AItem::AItem() :
	AActor(),
	ItemName("Default"),
	ItemCount{ 0 },
	ItemRarity{ EItemRarity::EIR_MIN },
	ItemState{ EItemState::EIS_PickUp },

	bItemInterp{ false },
	ItemInterpTime{ 0.75f }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetEnableGravity(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	PickUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidgetComponent"));
	PickUpWidgetComponent->SetupAttachment(GetRootComponent());

	WidgetCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WidgetCollision"));
	WidgetCollision->SetupAttachment(GetRootComponent());
	WidgetCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WidgetCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WidgetCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	WidgetTraceCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WidgetTraceCollision"));
	WidgetTraceCollision->SetupAttachment(GetRootComponent());
	WidgetTraceCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WidgetTraceCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WidgetTraceCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WidgetTraceCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	UPickUpWidget* PickUpWidget = Cast<UPickUpWidget>(PickUpWidgetComponent->GetUserWidgetObject());
	if (PickUpWidget) {
		PickUpWidget->SetWidgetOwner(this);
	}
	PickUpWidgetComponent->SetVisibility(false);

	SetItemState(EItemState::EIS_PickUp);

	WidgetTraceCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::WidgetTraceCollisionOB);
	WidgetTraceCollision->OnComponentEndOverlap.AddDynamic(this, &AItem::WidgetTraceCollisionOE);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ItemInterp(DeltaTime);
}

void AItem::SetItemState(EItemState State) {
	switch (State) {
		case EItemState::EIS_PickUp :
			ItemMesh->SetSimulatePhysics(false);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			WidgetCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			WidgetTraceCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			break;
		case EItemState::EIS_Equipping :
			ItemMesh->SetSimulatePhysics(false);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			WidgetCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			WidgetTraceCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EItemState::EIS_Equipped :
			ItemMesh->SetSimulatePhysics(false);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			WidgetCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			WidgetTraceCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EItemState::EIS_Falling :
			ItemMesh->SetSimulatePhysics(true);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

			WidgetCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			WidgetTraceCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EItemState::EIS_PickedUp :
			break;
	}

	ItemState = State;
}

void AItem::WidgetTraceCollisionOB(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AShutarCharacter* ShutarCharacter = Cast<AShutarCharacter>(OtherActor);
		if (ShutarCharacter) {
			ShutarCharacter->IncrementItemOverlappingCount(1);
		}
	}
}
void AItem::WidgetTraceCollisionOE(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		AShutarCharacter* ShutarCharacter = Cast<AShutarCharacter>(OtherActor);
		if (ShutarCharacter) {
			ShutarCharacter->IncrementItemOverlappingCount(-1);
		}
	}
}

void AItem::ItemInterpStart(AShutarCharacter* Char) {
	ItemInterpTargetChar = Char;
	ItemLocation = GetActorLocation();

	SetItemState(EItemState::EIS_Equipping);
	bItemInterp = true;
	GetWorldTimerManager().SetTimer(ItemInterpTimer, this, &AItem::ItemInterpDisable, ItemInterpTime);
}
void AItem::ItemInterpDisable() {
	bItemInterp = false;
	ItemInterpTargetChar->EquipItem(this);
	ItemInterpTargetChar = nullptr;
}

void AItem::ItemInterp(float DeltaTime) {
	if (!bItemInterp) return;

	if (ItemInterpTargetChar && ItemInterpZCurve) {
		FVector CurrentItemLocation = GetActorLocation();
		FVector TargetLocation = ItemInterpTargetChar->GetItemInterpTargetLocation();

		FVector InterpedLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 30.f);
		FRotator InterpedRotation = ItemInterpTargetChar->GetIteminterpTargeTRotation();

		float ItemToTargetZDifference = (TargetLocation.Z > ItemLocation.Z) ? TargetLocation.Z - ItemLocation.Z : ItemLocation.Z - TargetLocation.Z;
		float CurrentTimer = GetWorldTimerManager().GetTimerElapsed(ItemInterpTimer);
		InterpedLocation.Z += ItemToTargetZDifference * ItemInterpZCurve->GetFloatValue(CurrentTimer);

		SetActorLocation(InterpedLocation);
		SetActorRotation(InterpedRotation);
	}
}
