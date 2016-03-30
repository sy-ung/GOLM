// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/SceneCapture2D.h"
#include "GOLMMiniMapCamera.generated.h"

/**
 * 
 */
UCLASS()
class GOLM_API AGOLMMiniMapCamera : public ASceneCapture2D
{
	GENERATED_BODY()

	class AGOLMCharacter *PlayerCharacter;
	

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CameraData) float Height;


	void SetPlayerCharacter(AGOLMCharacter *Player);
	void Zoom(float value);
	
	void UpdateCamera();

	
	
};
