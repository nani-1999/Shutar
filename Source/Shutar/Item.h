// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM()
enum class EItemRarity : uint8 {
	EIR_MIN,
	EIR_Damaged = 1,
	EIR_Common = 2,
	EIR_UnCommon = 3,
	EIR_Rare = 4,
	EIR_Legendary = 5,
	EIR_MAX
};

UENUM()
enum class EItemState : uint8 {
	EIS_MIN,
	EIS_PickUp,
	EIS_Equipping,
	EIS_Equipped,
	EIS_Falling,
	EIS_PickedUp,
	EIS_MAX
};

UCLASS()
class SHUTAR_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnywhere, Category = "Item | Mesh", meta = (AllowPrivateAccess))
	class USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item | Widget", meta = (AllowPrivateAccess))
	class UBoxComponent* WidgetCollision;

	UPROPERTY(VisibleAnywhere, Category = "Item | Widget", meta = (AllowPrivateAccess))
	class UWidgetComponent* PickUpWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Item | Widget", meta = (AllowPrivateAccess))
	class USphereComponent* WidgetTraceCollision;

	//Item Data
	UPROPERTY(EditAnywhere, Category = "Item | Data", meta = (AllowPrivateAccess))
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Item | Data", meta = (AllowPrivateAccess))
	int32 ItemCount;

	UPROPERTY(EditAnywhere, Category = "Item | Data", meta = (AllowPrivateAccess))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, Category = "Item | Data", meta = (AllowPrivateAccess))
	EItemState ItemState;

	//functions
	UFUNCTION()
	void WidgetTraceCollisionOB(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WidgetTraceCollisionOE(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Item Interpolation
	class AShutarCharacter* ItemInterpTargetChar;
	bool bItemInterp;
	void ItemInterp(float DeltaTime);

	FTimerHandle ItemInterpTimer;
	float ItemInterpTime;
	void ItemInterpDisable();

	UPROPERTY(EditAnywhere, Category = "Item | Interpolation", meta = (AllowPrivateAccess))
	class UCurveFloat* ItemInterpZCurve;
	FVector ItemLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Getters
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE UWidgetComponent* GetPickUpWidgetComponent() const { return PickUpWidgetComponent; }
	FORCEINLINE FString GetItemName() const { return ItemName; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	FORCEINLINE EItemRarity GetItemRarity() const { return ItemRarity; }
	//Setters
	void SetItemState(EItemState State);

	void ItemInterpStart(AShutarCharacter* Char);
};
