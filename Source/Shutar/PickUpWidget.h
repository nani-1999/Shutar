// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHUTAR_API UPickUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//Data
	UPROPERTY(VisibleAnywhere, Category = "PickUpWidget | Data", meta = (AllowPrivateAccess))
	class AItem* WidgetOwner;

	//Widgets
	UPROPERTY(meta = (BindWidget))
	class UImage* Star1Icon;
	UPROPERTY(meta = (BindWidget))
	class UImage* Star2Icon;
	UPROPERTY(meta = (BindWidget))
	class UImage* Star3Icon;
	UPROPERTY(meta = (BindWidget))
	class UImage* Star4Icon;
	UPROPERTY(meta = (BindWidget))
	class UImage* Star5Icon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCount;

public:
	FORCEINLINE void SetWidgetOwner(AItem* OwnerToSet) { WidgetOwner = OwnerToSet; }
};
