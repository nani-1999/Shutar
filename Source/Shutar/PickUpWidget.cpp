// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpWidget.h"
#include "Item.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPickUpWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (WidgetOwner) {
		if (ItemName) ItemName->SetText(FText::FromString(WidgetOwner->GetItemName()));
		if (ItemCount) ItemCount->SetText(FText::AsNumber(WidgetOwner->GetItemCount()));

		int8 ItemRarity = (int8)WidgetOwner->GetItemRarity();
		if (Star1Icon && ItemRarity < 1) Star1Icon->SetOpacity(0); else Star1Icon->SetOpacity(1);
		if (Star2Icon && ItemRarity < 2) Star2Icon->SetOpacity(0); else Star2Icon->SetOpacity(1);
		if (Star3Icon && ItemRarity < 3) Star3Icon->SetOpacity(0); else Star3Icon->SetOpacity(1);
		if (Star4Icon && ItemRarity < 4) Star4Icon->SetOpacity(0); else Star4Icon->SetOpacity(1);
		if (Star5Icon && ItemRarity < 5) Star5Icon->SetOpacity(0); else Star5Icon->SetOpacity(1);
	}
}

void UPickUpWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
}