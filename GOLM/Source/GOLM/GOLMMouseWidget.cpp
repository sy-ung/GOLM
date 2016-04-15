// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMMouseWidget.h"


void UGOLMMouseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	

}

void UGOLMMouseWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGOLMMouseWidget::MoveMouseCursor(class AGOLMPlayerController *PlayerController)
{
	if(PlayerController != NULL)
	{
		float MouseX, MouseY;
		PlayerController->GetMousePosition(MouseX, MouseY);
		FVector2D MouseSize = FVector2D::ZeroVector;
		if(IsValid(MouseCursor))
			MouseSize = MouseCursor->Brush.ImageSize / 2;

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
}

void UGOLMMouseWidget::SetMouseReference(UImage *MouseReference)
{
	MouseCursor = MouseReference;
}

UImage *UGOLMMouseWidget::GetMouseReference()
{
	return MouseCursor;
}

void UGOLMMouseWidget::ChangeMouseType(EPlayerCursorType NewCursor)
{
	if (MouseCursor == NULL)
		return;

	switch (NewCursor)
	{
	case EPlayerCursorType::CROSSHAIR:
		MouseCursor->SetBrushFromTexture(Crosshair);
		break;
	case EPlayerCursorType::MENU:
		MouseCursor->SetBrushFromTexture(MenuCursor);
	default:break;
	}
}
