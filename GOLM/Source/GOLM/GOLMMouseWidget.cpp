// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMPlayerController.h"
#include "GOLMMouseWidget.h"


void UGOLMMouseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<AGOLMPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}

void UGOLMMouseWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);
	FVector2D MouseSize = MouseCursor->Brush.ImageSize / 2;

	switch (PlayerController->CurrentCursorType)
	{

	case EPlayerCursorType::CROSSHAIR:
		SetPositionInViewport(FVector2D(MouseX - MouseSize.X, MouseY - MouseSize.Y));
		break;
	case EPlayerCursorType::MENU:
		SetPositionInViewport(FVector2D(MouseX, MouseY));
		break;

	default:
		break;
	}

	

}

void UGOLMMouseWidget::SetMouseReference(UImage *MouseReference)
{
	MouseCursor = MouseReference;
}

UImage *UGOLMMouseWidget::GetMouseReference()
{
	return MouseCursor;
}
