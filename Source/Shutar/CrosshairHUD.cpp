// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairHUD.h"
#include "ShutarCharacter.h"
#include "Kismet/GameplayStatics.h"

ACrosshairHUD::ACrosshairHUD() :
	AHUD(),
	TextureSize(64.f),
	CrosshairSpreadMax(16.f) //quarter of TextureSize
{
	PrimaryActorTick.bCanEverTick = false;

}

void ACrosshairHUD::BeginPlay() {
	Super::BeginPlay();

	if (ShutarCharacter == nullptr) {
		ShutarCharacter = Cast<AShutarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
}

void ACrosshairHUD::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACrosshairHUD::DrawHUD() {
	Super::DrawHUD();

	if (ShutarCharacter) {
		FVector2D CrosshairScreenLocation;
		if (!ShutarCharacter->GetCrosshairScreenLocation(CrosshairScreenLocation)) return;

		FVector2D CrosshairTextureLocation = CrosshairScreenLocation - (TextureSize / 2.f);

		float CrosshairSpreadRate = ShutarCharacter->GetCrosshairSpreadRate();

		if (CrosshairTextureMiddle) DrawTexture(CrosshairTextureMiddle, CrosshairTextureLocation.X, CrosshairTextureLocation.Y, TextureSize, TextureSize, 0.f, 0.f, 1.f, 1.f);

		float CrosshairTextureTopLocationY = CrosshairTextureLocation.Y - (CrosshairSpreadMax * CrosshairSpreadRate);
		if (CrosshairTextureTop) DrawTexture(CrosshairTextureTop, CrosshairTextureLocation.X, CrosshairTextureTopLocationY, TextureSize, TextureSize, 0.f, 0.f, 1.f, 1.f);

		float CrosshairTextureRightLocationX = CrosshairTextureLocation.X + (CrosshairSpreadMax * CrosshairSpreadRate);
		if (CrosshairTextureRight) DrawTexture(CrosshairTextureRight, CrosshairTextureRightLocationX, CrosshairTextureLocation.Y, TextureSize, TextureSize, 0.f, 0.f, 1.f, 1.f);

		float CrosshairTextureBottomLocationY = CrosshairTextureLocation.Y + (CrosshairSpreadMax * CrosshairSpreadRate);
		if (CrosshairTextureBottom) DrawTexture(CrosshairTextureBottom, CrosshairTextureLocation.X, CrosshairTextureBottomLocationY, TextureSize, TextureSize, 0.f, 0.f, 1.f, 1.f);

		float CrosshairTextureLeftLocationX = CrosshairTextureLocation.X - (CrosshairSpreadMax * CrosshairSpreadRate);
		if (CrosshairTextureLeft) DrawTexture(CrosshairTextureLeft, CrosshairTextureLeftLocationX, CrosshairTextureLocation.Y, TextureSize, TextureSize, 0.f, 0.f, 1.f, 1.f);
	}
}