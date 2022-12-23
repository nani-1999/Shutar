// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrosshairHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHUTAR_API ACrosshairHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ACrosshairHUD();

	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

	//properties
	UPROPERTY(VisibleAnywhere, Category = "CrosshairHUD | Data")
	class AShutarCharacter* ShutarCharacter;

	UPROPERTY(EditAnywhere, Category = "CrosshairHUD | Texture", meta = (AllowPrivateAccess))
	class UTexture* CrosshairTexture;
	UPROPERTY(EditAnywhere, Category = "CrosshairHUD | Texture", meta = (AllowPrivateAccess))
	class UTexture* CrosshairTextureMiddle;
	UPROPERTY(EditAnywhere, Category = "CrosshairHUD | Texture", meta = (AllowPrivateAccess))
	class UTexture* CrosshairTextureTop;
	UPROPERTY(EditAnywhere, Category = "CrosshairHUD | Texture", meta = (AllowPrivateAccess))
	class UTexture* CrosshairTextureRight;
	UPROPERTY(EditAnywhere, Category = "CrosshairHUD | Texture", meta = (AllowPrivateAccess))
	class UTexture* CrosshairTextureBottom;
	UPROPERTY(EditAnywhere, Category = "CrosshairHUD | Texture", meta = (AllowPrivateAccess))
	class UTexture* CrosshairTextureLeft;

	UPROPERTY(EditAnywhere, Category = "CrosshairHUD | Texture", meta = (AllowPrivateAccess))
	float TextureSize;

	UPROPERTY(VisibleAnywhere, Category = "CrosshairHUD | Texture", meta = (AllowPrivateAccess))
	float CrosshairSpreadMax;

public:
	virtual void Tick(float DeltaTime) override;
};
